#include "client/net/BaseClient.hpp"

#include <iostream>


namespace net {

BaseClient::BaseClient() {
    Py_InitializeEx(0);
    PyEval_InitThreads();
    PyList_Insert(PySys_GetObject("path"),
                  0, PyUnicode_FromString("../python/"));
    auto zm = PyImport_ImportModule("zm");
    if (!zm) {
        // TODO: Check error and die
    }
    auto zm_main = PyObject_GetAttr(zm, PyUnicode_FromString("main"));
    if (!zm_main || !PyCallable_Check(zm_main)) {
        std::cout << "Python entry point (zm.main) does not exist or is not callable";
    }
    auto zm_main_rv = PyObject_CallFunction(zm_main, "");
    if (!zm_main_rv) {
        auto exc = PyErr_Occurred();
        if (exc) {
            PyErr_Print();
        }
    } else {
        fprintf(stderr, "zm.main() -> ");
        PyObject_Print(zm_main_rv, stderr, Py_PRINT_RAW);
        fprintf(stderr, "\n");
        m_py_client = zm_main_rv;
        m_py_o_messages = PyObject_GetAttrString(m_py_client, "in_messages");
        m_py_messages = PyObject_GetAttr(
            zm_main_rv, PyUnicode_FromString("messages"));
    }
    m_tstate_count = 0;
    m_tstate = PyEval_SaveThread();
}


void BaseClient::saveThread() {
    if (m_tstate_count) {
        // Should probably raise an exception here if count is already 0
        // as it indicates a programming error.
        m_tstate_count--;
        if (m_tstate_count == 0) {
            m_tstate = PyEval_SaveThread();
        }
    }
}


void BaseClient::restoreThread() {
    m_tstate_count++;
    if (m_tstate_count > 0 && m_tstate) {
        PyEval_RestoreThread(m_tstate);
        m_tstate = NULL;
    }
}


BaseClient::~BaseClient() {
    restoreThread();
    auto tid = PyObject_GetAttr(m_py_client, PyUnicode_FromString("ident"));
    PyThreadState_SetAsyncExc(PyLong_AsLong(tid), PyExc_SystemExit);
    Py_Finalize();
}


void BaseClient::pump() {
    restoreThread();
    auto nmessages = PyList_Size(m_py_messages);
    for (auto i = 0; i < nmessages; i++) {
        auto message = PyList_GetItem(m_py_messages, i);
        auto type = std::string(PyUnicode_AsUTF8(
                PyObject_GetAttr(message, PyUnicode_FromString("type"))));
        auto entity = PyObject_GetAttr(message,
            PyUnicode_FromString("entity"));
        convert(type, entity);
    }
    PySequence_DelSlice(m_py_messages, 0, nmessages);
    saveThread();
}


void BaseClient::pushMessage(std::string type, PyObject* entity) {
    restoreThread();
    auto message = Py_BuildValue("(sO)", type.c_str(), entity);
    Py_DECREF(entity);
    if (!message) {
        PyErr_Print();
    } else {
        PyList_Append(m_py_o_messages, message);
    }
    saveThread();
}


}  // namespace net
