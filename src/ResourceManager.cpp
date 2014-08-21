#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

ResourceManager::ResourceManager() {
    // Load the spritesheets n shit
    m_sheets.emplace(std::piecewise_construct, std::forward_as_tuple("main"),
                     std::forward_as_tuple("resources/spritesheet.png", 8));
}

SpriteSheet &ResourceManager::getSheet(char const *const key) {
    auto iter = m_sheets.find(key);

    if (iter == m_sheets.end()) {
        // Throw a runtime error if we couldn't find the spritesheet.
        std::string error("Fuck I couldn't find spritesheet ");
        error += key;
        throw std::runtime_error(error);
    }

    return iter->second;
}
