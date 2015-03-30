#pragma once

#include "sys/Texture.hpp"

#include "ResourceCollection.hpp"

#include "resources/SpriteResource.hpp"
#include "resources/MusicResource.hpp"

#include <unordered_map>

#include "common/star/tarlib.hpp"

#include <json11.hpp>
#include <format.h>

#include <SDL_mixer.h>

namespace client {

class ResourceManager {
public:
    /// Initialize the resources
    ResourceManager(std::string resource_package);
    /// Get a spritesheet by its name.
    sys::Texture & getTexture(char const * const key);
    // Load a package from disk.
    void loadPackage(std::string resource_package, PackageType type);
    void loadTexture(Tar tar, std::string path);
    void loadMusic(Tar tar, std::string path);
    Mix_Music* getMusic(char const * const key);

public:
    ResourceCollection<SpriteResource> m_sprites;
    ResourceCollection<MusicResource>  m_music;

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
    std::unordered_map<std::string, Mix_Music*>   m_musicfiles;
};
} // namespace client
