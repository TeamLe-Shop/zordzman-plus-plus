#pragma once

#include "sys/Texture.hpp"

#include "common/resources/ResourceCollection.hpp"
#include "common/resources/SpriteResource.hpp"
#include "common/resources/MusicResource.hpp"
#include "common/resources/FontResource.hpp"
#include "common/resources/SoundResource.hpp"

#include <unordered_map>

#include "common/star/tarlib.hpp"

#include <json11.hpp>
#include <format.h>

#include <SDL_mixer.h>
#include <SDL_ttf.h>

namespace client {

using namespace resources;

class ResourceManager {
public:
    /// Initialize the resources
    ResourceManager(std::string resource_package);
    // Load a package from disk.
    void loadPackage(std::string resource_package, resources::PackageType type);

    void loadTexture(Tar tar, std::string path);
    sys::Texture & getTexture(std::string key);

    void loadMusic(Tar tar, std::string path);
    Mix_Music* getMusic(std::string key);

    void loadFont(Tar tar, std::string path, size_t size);
    TTF_Font* getFont(std::string key);

    void loadSound(Tar tar, std::string path);
    Mix_Chunk* getSound(std::string key);

public:
    ResourceCollection<SpriteResource> m_sprites;
    ResourceCollection<MusicResource>  m_music;
    ResourceCollection<FontResource>   m_fonts;
    ResourceCollection<SoundResource>  m_sounds;

private:
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager operator=(ResourceManager const &) = delete;
    std::unordered_map<std::string, sys::Texture> m_textures;
    std::unordered_map<std::string, Mix_Music*>   m_musicfiles;
    std::unordered_map<std::string, TTF_Font*>    m_fontlist;
    std::unordered_map<std::string, Mix_Chunk*>   m_soundlist;
};
} // namespace client
