#include <iostream>
#include <stdexcept>

#include "client/net/BaseClient.hpp"
#include "common/extlib/cppformat/format.h"


namespace net {

BaseClient::BaseClient() {
    Py_InitializeEx(0);
    PyEval_InitThreads();
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
    const char * ep_error = NULL;
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

bool BaseClient::invokeEntryPoint(PyObject * ep, const char ** error) {
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


void BaseClient::process() {
    restoreThread();
    PyObject * message;
    PyObject * iterator = PyObject_CallFunction(m_py_client_retrieve, "");
    if (!iterator) {
        PyErr_Print();
        saveThread();
        return;
    }
    if (!PyIter_Check(iterator)) {
        saveThread();
        throw std::runtime_error("Client.retrieve did not return an iterator");
    }
    while ((message = PyIter_Next(iterator))) {
        std::string type("");
        PyObject * py_type_name = NULL;
        PyObject * py_entity = PyObject_GetAttrString(message, "entity");
        PyObject * py_type = PyObject_GetAttrString(message, "type");
        if (py_type) {
            py_type_name = PyObject_GetAttrString(py_type, "name");
        }
        if (py_type_name) {
            // "The caller is not responsible for deallocating the buffer."
            //      - PyUnicode documentation
            auto c_name = PyUnicode_AsUTF8(py_type_name);
            if (c_name) {
                type += c_name;
            }
        }
        if (!PyErr_Occurred()) {
            processMessage(type, py_entity);
        } else {
            PyErr_Print();
        }
        Py_XDECREF(py_type);
        Py_XDECREF(py_type_name);
        Py_XDECREF(py_entity);
        Py_DECREF(message);
    }
    Py_DECREF(iterator);
    saveThread();
}


// TODO: Better error handling
void BaseClient::send(std::string type, PyObject* entity) {
    restoreThread();
    auto args = Py_BuildValue("(sO)", type.c_str(), entity);
    Py_DECREF(entity);
    if (!args) {
        PyErr_Print();
        saveThread();
        return;
    }
    PyObject_CallObject(m_py_client_send, args);
    Py_DECREF(args);
    if (PyErr_Occurred()) {
        PyErr_Print();
        saveThread();
        return;
    }
    saveThread();
}


}  // namespace net
