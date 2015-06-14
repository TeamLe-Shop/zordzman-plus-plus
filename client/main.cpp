#include "Client.hpp"

#include <stdexcept>
#include <clocale>
#include <iostream>

#include "format.h"
#include "Config.hpp"
#include "HUD.hpp"
#include "net/Client.hpp"
#include <thread>
#include "net/messages.hpp"
using namespace client;


void es_handler(net::ingress::EntityState state) {
    fprintf(
        stderr,
        "HI I'M NOLHANDLER: %li.%s.%s = (%p) ",
        state.id,
        state.component.c_str(),
        state.field.c_str(),
        state.value
    );
    PyObject_Print(state.value, stderr, Py_PRINT_RAW);
    fprintf(stderr, "\n");
}


void handler(net::ingress::zm::client::Connected) {
    fprintf(stderr, "Connected!\n");
}


int main(int argc, char * argv[]) {
    net::Client client;
    client.addHandler(handler);
    client.addHandler(es_handler);
    client.send(net::egress::zm::client::Connect({"127.0.0.1", 9001}));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    client.process();
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
