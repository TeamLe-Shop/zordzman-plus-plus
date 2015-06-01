#include "Client.hpp"

#include <stdexcept>
#include <clocale>
#include <iostream>

#include "format.h"
#include "Config.hpp"
#include "HUD.hpp"
#include "python/python_i.hpp"
#include <thread>
using namespace client;


void handler(python::EntityState state) {
    fprintf(
        stderr,
        "HI I'M NOLHANDLER: %i.%s.%s = ",
        state.id,
        state.component.c_str(),
        state.field.c_str()
    );
    PyObject_Print(state.value, stderr, Py_PRINT_RAW);
    fprintf(stderr, "\n");
}


int main(int argc, char * argv[]) {
    python::Client client;
    client.onEntityState(handler);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    client.pump();
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
