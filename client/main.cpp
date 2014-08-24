#include "Client.hpp"

#include <stdexcept>
#include "format.h"
#include "Config.hpp"

using namespace client;

int main(int argc, char *argv[]) {
    try {
        Config cfg;
        if (argc > 1) {
            cfg.host = argv[1];
        }
        // Initialize the game.
        Client game(cfg);
        // Start the game loop.
        game.exec();
    }
    catch (std::exception const &except) {
        fmt::print(stderr, "Fatal exception: {}\n", except.what());
        return 1;
    }
}
