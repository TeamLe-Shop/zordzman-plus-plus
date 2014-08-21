#include "Game.hpp"

#include <stdexcept>
#include "format.h"

int main() {
    try {
        // Initialize the game.
        Game game;
        // Start the game loop.
        game.exec();
    }
    catch (std::exception const &except) {
        fmt::print(stderr, "Fatal exception: {}\n", except.what());
        return 1;
    }
}
