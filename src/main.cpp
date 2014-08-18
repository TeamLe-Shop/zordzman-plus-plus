#include "Game.hpp"

#include <stdexcept>
#include <iostream>

int main() {
    try {
        // Initialize the game.
        Game game;
        // Start the game loop.
        game.exec();
    }
    catch (std::exception const &except) {
        std::cerr << "Fatal exception: " << except.what();
        return 1;
    }
}
