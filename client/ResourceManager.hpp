#pragma once
#include "sys/Texture.hpp"

#include <unordered_map>

namespace client {
class ResourceManager {
public:
    /// @brief Initialize the resources.
    ResourceManager();
    /// @brief Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
};
}
