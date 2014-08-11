#include "Game.hpp"

#include <stdexcept>
#include <iostream>

int main() {
    try {
        Game game;
        game.exec();
    }
    catch (std::exception const &except) {
        std::cerr << "Fatal exception: " << except.what();
        return 1;
    }
}
