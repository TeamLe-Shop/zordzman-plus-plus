#pragma once

#include "sys/Texture.hpp"

#include <unordered_map>

#include "common/star/tarlib.hpp"

#include <json11.hpp>

namespace client {

struct SpriteData {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    std::string path;
};

class ResourceManager {
public:
    /// Initialize the resources.
    ResourceManager();
    /// Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);
    /// Load in a sprite by Json object.
    void loadSprite(json11::Json json, Tar tar);

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
    std::unordered_map<std::string, SpriteData> m_spritedata;
};
} // namespace client
