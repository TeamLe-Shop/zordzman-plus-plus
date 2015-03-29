#pragma once

#include "sys/Texture.hpp"

#include "ResourceCollection.hpp"

#include "resources/SpriteResource.hpp"
#include "resources/MusicResource.hpp"

#include <unordered_map>

#include "common/star/tarlib.hpp"

#include <json11.hpp>
#include <format.h>

namespace client {

class ResourceManager {
public:
    /// Initialize the resources
    ResourceManager(std::string resource_package);
    /// Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);
    // Load a package from disk.
    void loadPackage(std::string resource_package, PackageType type);

public:
    ResourceCollection<SpriteResource> m_sprites;
    ResourceCollection<MusicResource>  m_music;

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
};
} // namespace client
