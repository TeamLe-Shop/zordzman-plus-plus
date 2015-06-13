#include <iostream>
#include <stdexcept>

#include "client/net/BaseClient.hpp"
#include "common/extlib/cppformat/format.h"


namespace net {

BaseClient::BaseClient() {
    Py_InitializeEx(0);
    PyEval_InitThreads();
    PyList_Insert(PySys_GetObject("path"),
                  0, PyUnicode_FromString("../python/"));
    auto zm = PyImport_ImportModule("zm");
    if (!zm) {
        PyErr_Print();
        throw std::runtime_error("Could not find the 'zm' package");
    }
    auto zm_client = PyObject_GetAttrString(zm, "spawn_client");
    if (!zm_client) {
        PyErr_Print();
        Py_DECREF(zm);
        throw std::runtime_error(
            "Python entry point (zm.client) does not exist");
    }
    char * ep_error = NULL;
    auto success = invokeEntryPoint(zm_client, &ep_error);
    Py_DECREF(zm);
    Py_DECREF(zm_client);
    if (!success) {
        throw std::runtime_error(ep_error);
    }
    m_tstate_count = 0;
    m_tstate = PyEval_SaveThread();
}


#define EP_FAIL(message) *error = message; PyErr_Print(); return false
#define EP_SUCCESS return true

bool BaseClient::invokeEntryPoint(PyObject * ep, char ** error) {
    *error = NULL;
    if (!PyCallable_Check(ep)) {
        EP_FAIL("Entry point is not callable");
    }
    m_py_client = PyObject_CallFunction(ep, "");
    if (!m_py_client) {
        EP_FAIL("Error calling entry point");
    }
    auto ident = PyObject_GetAttrString(m_py_client, "ident");
    if (ident) {
        // Can't overflow; PyThreadState_SetAsyncExc takes a long ;)
        m_tid = PyLong_AsLong(ident);
        Py_DECREF(ident);
    } else {
        Py_DECREF(m_py_client);
        EP_FAIL("Entry point didn't return a thread object");
    }
    m_py_client_send = PyObject_GetAttrString(m_py_client, "send");
    if (m_py_client_send) {
        if (!PyCallable_Check(m_py_client_send)) {
            Py_DECREF(m_py_client);
            Py_DECREF(m_py_client_send);
            EP_FAIL("Client.send is not callable");
        }
    } else {
        Py_DECREF(m_py_client);
        EP_FAIL("Client has no attribute 'send'");
    }
    m_py_client_retrieve = PyObject_GetAttrString(m_py_client, "retrieve");
    if (m_py_client_retrieve) {
        if (!PyCallable_Check(m_py_client_retrieve)) {
            Py_DECREF(m_py_client);
            Py_DECREF(m_py_client_send);
            Py_DECREF(m_py_client_retrieve);
            EP_FAIL("Client.retrieve is not callable");
        }
    } else {
        Py_DECREF(m_py_client);
        Py_DECREF(m_py_client_send);
        EP_FAIL("Client has no attribute 'retrieve'");
    }
    EP_SUCCESS;
}

#undef EP_FAIL
#undef EP_SUCCESS


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
    PyThreadState_SetAsyncExc(m_tid, PyExc_SystemExit);
    Py_DECREF(m_py_client);
    Py_DECREF(m_py_client_send);
    Py_DECREF(m_py_client_retrieve);
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
