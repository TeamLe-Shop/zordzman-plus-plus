#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

#include "common/resources/ResourcePackage.hpp"

#include "sys/Texture.hpp"

using namespace common::util;

namespace client {
ResourceManager::ResourceManager(std::string base_resource) {
    loadPackage(base_resource, Base);
}

void ResourceManager::loadPackage(std::string resource_package,
                                  PackageType type) {
    ResourcePackage rpackage(resource_package, type);

    m_sprites.loadPackage(rpackage);
    m_music.loadPackage(rpackage);
    m_fonts.loadPackage(rpackage);

    debug("Loaded sprites:\n");
    for (auto package : m_sprites.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto sprite = data.second;
            debug("  ({}) {}x{}, {}, {} on spritesheet {}\n", data.first,
                  sprite.m_width, sprite.m_height,
                  sprite.m_x, sprite.m_y, sprite.m_path);
            loadTexture(rpackage.getTar(), sprite.m_path);
        }
    }

    debug("Loaded music:\n");
    for (auto package : m_music.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto music = data.second;
            debug("  ({}) in file {}\n", data.first, music.m_path);
            // Currently non-JIT loading
            loadMusic(rpackage.getTar(), music.m_path);
        }
    }

    debug("Loaded fonts:\n");
    for (auto package : m_fonts.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto font = data.second;
            debug("  ({}) in file {}\n", data.first, font.m_path);
            loadFont(rpackage.getTar(), font.m_path, font.m_size);
        }
    }

    debug("Loaded sounds:\n");
    for (auto package : m_sounds.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto sound = data.second;
            debug("  ({}) in file {}\n", data.first, sound.m_path);
            // Currently non-JIT loading
            loadSound(rpackage.getTar(), sound.m_path);
        }
    }

    debug("Loaded textures:\n");
    for (const auto & texture : m_textures) {
        debug("  Path: {}\n", texture.first);
    }

    debug("Loaded music:\n");
    for (const auto & music : m_musicfiles) {
        debug("  Path: {}\n", music.first);
    }

    debug("Loaded sounds:\n");
    for (const auto & sound : m_soundlist) {
        debug("  Path: {}\n", sound.first);
    }
}

sys::Texture & ResourceManager::getTexture(std::string key) {
    auto iter = m_textures.find(key);

    if (iter == m_textures.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find texture \"{}\"", key));
    }

    return iter->second;
}

Mix_Music* ResourceManager::getMusic(std::string key) {
    auto iter = m_musicfiles.find(key);

    if (iter == m_musicfiles.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find music \"{}\"", key));
    }

    return iter->second;
}

TTF_Font* ResourceManager::getFont(std::string key) {
    auto iter = m_fontlist.find(key);

    if (iter == m_fontlist.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find font \"{}\"", key));
    }

    return iter->second;
}

Mix_Chunk* ResourceManager::getSound(std::string key) {
    auto iter = m_soundlist.find(key);

    if (iter == m_soundlist.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find sound \"{}\"", key));
    }

    return iter->second;
}

void ResourceManager::loadTexture(Tar tar, std::string path) {
    for (auto e : tar.getEntries()) {
        if (std::string(e->name) == path) {
            if (m_textures.find(path) == m_textures.end()) {
                m_textures.emplace(std::piecewise_construct,
                                   std::forward_as_tuple(path),
                                   std::forward_as_tuple(e->contents,
                                                         sys::Memory));
            }
        }
    }
}

void ResourceManager::loadMusic(Tar tar, std::string path) {
    for (auto e : tar.getEntries()) {
        if (std::string(e->name) == path) {
            if (m_musicfiles.find(path) == m_musicfiles.end()) {
                SDL_RWops *rwops
                    = SDL_RWFromMem(const_cast<char *>(e->contents.data()),
                                    e->contents.size());
                Mix_Music* music = Mix_LoadMUS_RW(rwops, 1);
                m_musicfiles.emplace(path, music);
            }
        }
    }
}

void ResourceManager::loadFont(Tar tar, std::string path, size_t size) {
    for (auto e : tar.getEntries()) {
        if (std::string(e->name) == path) {
            if (m_fontlist.find(path) == m_fontlist.end()) {
                SDL_RWops *rwops
                    = SDL_RWFromMem(const_cast<char *>(e->contents.data()),
                                    e->contents.size());
                TTF_Font* font = TTF_OpenFontRW(rwops, 1, size);
                m_fontlist.emplace(path, font);
            }
        }
    }
}

void ResourceManager::loadSound(Tar tar, std::string path) {
    for (auto e : tar.getEntries()) {
        if (std::string(e->name) == path) {
            if (m_soundlist.find(path) == m_soundlist.end()) {
                SDL_RWops *rwops
                    = SDL_RWFromMem(const_cast<char *>(e->contents.data()),
                                    e->contents.size());
                Mix_Chunk* sound = Mix_LoadWAV_RW(rwops, 1);
                m_soundlist.emplace(path, sound);
            }
        }
    }
}

} // namespace client
