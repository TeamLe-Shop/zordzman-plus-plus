#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

#include "ResourcePackage.hpp"

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


    debug("Loaded textures:\n");
    for (const auto & texture : m_textures) {
        debug("  Path: {}\n", texture.first);
    }

    debug("Loaded music:\n");
    for (const auto & music : m_musicfiles) {
        debug("  Path: {}\n", music.first);
    }
}

sys::Texture & ResourceManager::getTexture(char const * const key) {
    auto iter = m_textures.find(key);

    if (iter == m_textures.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find texture \"{}\"", key));
    }

    return iter->second;
}

Mix_Music* ResourceManager::getMusic(char const * const key) {
    auto iter = m_musicfiles.find(key);

    if (iter == m_musicfiles.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find music file \"{}\"", key));
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

} // namespace client
