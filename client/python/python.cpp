#include "client/python/python.hpp"

#include <iostream>


namespace python {

Client::Client() {
    Py_InitializeEx(0);
    PyList_Insert(PySys_GetObject("path"),
                  0, PyUnicode_FromString("../client/python/"));
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
    }
}


Client::~Client() {
    Py_Finalize();
}


}  // namespace python
