#include <Python.h>

#include "Client.hpp"

#include <stdexcept>
#include <clocale>
#include <iostream>

#include "format.h"
#include "Config.hpp"
#include "HUD.hpp"

using namespace client;


void start_zm_client() {
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
    Py_Finalize();
}


int main(int argc, char * argv[]) {
    start_zm_client();
    return 0;
    try {
        Config cfg("config/config.json");

        setlocale(LC_ALL, "");
        std::cout << setlocale(LC_CTYPE, NULL) << std::endl;

        // Load in the default HUD.
        // The contents of default_hud.json could just be '{}',
        // but all the default values are filled in there for
        // a customizer's reference.
        HUD hud("config/default_hud.json");

        if (argc > 1) {
            cfg.host = argv[1];
        }
        if (argc > 2) {
            cfg.port = std::stoi(argv[2]);
        }
        // Initialize the game.
        //Client game(cfg, hud);
        // Start the game loop.
        //game.exec();
    } catch (std::exception const & except) {
        fmt::print(stderr, "Fatal exception: {}\n", except.what());
        return 1;
    }
}
