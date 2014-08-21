#pragma once
#include "gfx/SpriteSheet.hpp"

#include <unordered_map>

class ResourceManager {
public:
    /// @brief Initialize the resources.
    ResourceManager();
    /// @brief Get a spritesheet by its name.
    SpriteSheet &getSheet(char const *const key);

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, SpriteSheet> m_sheets;
};
