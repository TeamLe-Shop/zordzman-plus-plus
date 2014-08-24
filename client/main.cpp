#include "Game.hpp"

#include <stdexcept>
#include "format.h"
#include "Config.hpp"
#include "HUDConfig.hpp"

int main(int argc, char *argv[]) {
    try {
        Config cfg;
        HUD hud("resources/default_hud.json");

        if (argc > 1) {
            cfg.host = argv[1];
        }
        // Initialize the game.
        Game game(cfg, hud);
        // Start the game loop.
        game.exec();
    }
    catch (std::exception const &except) {
        fmt::print(stderr, "Fatal exception: {}\n", except.what());
        return 1;
    }
}
