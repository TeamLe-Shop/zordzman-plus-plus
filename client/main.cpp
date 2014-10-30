#include "Client.hpp"

#include <stdexcept>
#include "format.h"
#include "Config.hpp"
#include "HUD.hpp"

using namespace client;

int main(int argc, char * argv[]) {
    try {
        Config cfg;

        // Load in the default HUD.
        // The contents of default_hud.json could just be '{}',
        // but all the default values are filled in there for
        // a customizer's reference.
        HUD hud("resources/default_hud.json");

        if (argc > 1) {
            cfg.host = argv[1];
        }
        if (argc > 2) {
            cfg.port = std::stoi(argv[2]);
        }
        // Initialize the game.
        Client game(cfg, hud);
        // Start the game loop.
        game.exec();
    } catch (std::exception const & except) {
        fmt::print(stderr, "Fatal exception: {}\n", except.what());
        return 1;
    }
}
