#include "globalResources.hpp"

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <tuple>

namespace globalResources {

std::unordered_map<std::string, SpriteSheet> sheets;

void init() {
    // Load the spritesheets n shit
    sheets.emplace(std::piecewise_construct, std::forward_as_tuple("main"),
                   std::forward_as_tuple("resources/spritesheet.png", 8));
}

SpriteSheet &getSheet(char const *const key) {
    auto iter = sheets.find(key);

    if (iter == sheets.end()) {
        // Throw a runtime error if we couldn't find the spritesheet.
        std::string error("Fuck I couldn't find spritesheet ");
        error += key;
        throw std::runtime_error(error);
    }

    return iter->second;
}

void free() { sheets.clear(); }
}
