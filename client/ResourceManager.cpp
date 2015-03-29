#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

#include "ResourcePackage.hpp"

using namespace common::util;

namespace client {
ResourceManager::ResourceManager(std::string base_resource) {
    loadPackage(base_resource, Base);

    debug("Loaded sprites:\n");
    for (auto package : m_sprites.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto sprite = data.second;
            debug("  ({}) {}x{}, {}, {} on spritesheet {}\n", data.first,
                  sprite.m_width, sprite.m_height,
                  sprite.m_x, sprite.m_y, sprite.m_path);
        }
    }

    debug("Loaded music:\n");
    for (auto package : m_music.getPackages()) {
        debug("Package {} ({})\n", package.getName(), package.getType());
        for (auto data : package.getResources()) {
            auto music = data.second;
            debug("  ({}) in file {}\n", data.first, music.m_path);
        }
    }


    debug("Loaded textures:\n");
    for (const auto & texture : m_textures) {
        debug("  Path: {}\n", texture.first);
    }
}

void ResourceManager::loadPackage(std::string resource_package,
                                  PackageType type) {
    ResourcePackage package(resource_package, type);
    m_sprites.loadPackage(package);
    m_music.loadPackage(package);
}

sys::Texture & ResourceManager::getTexture(char const * const key) {
    auto iter = m_textures.find(key);

    if (iter == m_textures.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find texture \"{}\"", key));
    }

    return iter->second;
}

} // namespace client
