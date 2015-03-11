#pragma once

#include "sys/Texture.hpp"

#include "ResourceCollection.hpp"

#include "resources/SpriteResource.hpp"

#include <unordered_map>

#include "common/star/tarlib.hpp"

#include <json11.hpp>
#include <format.h>

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
    ResourceManager(std::string resource_package);
    /// Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);

public:
    ResourceCollection<SpriteResource> m_sprites;

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
};
} // namespace client
