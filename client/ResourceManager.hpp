#pragma once
#include "sys/Texture.hpp"

#include <unordered_map>

namespace client {
class ResourceManager {
public:
    /// Initialize the resources.
    ResourceManager();
    /// Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
};
} // namespace client
