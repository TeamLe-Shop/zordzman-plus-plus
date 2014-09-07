#pragma once
#include "gfx/SpriteSheet.hpp"

#include <unordered_map>

namespace client {
class ResourceManager {
public:
    /// @brief Initialize the resources.
    ResourceManager();
    /// @brief Get a spritesheet by its name.
    SpriteSheet & getSheet(char const * const key);

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    // The map containing spritesheets that can be referred to by their name.
    std::unordered_map<std::string, SpriteSheet> m_sheets;
};
}
