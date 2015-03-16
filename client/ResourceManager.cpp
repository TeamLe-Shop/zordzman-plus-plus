#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

#include "resources/SpriteResource.hpp"

using namespace common::util;

namespace client {
ResourceManager::ResourceManager(std::string base_resource) {
    loadPackage(base_resource);

    debug("Loaded sprites:\n");
    for (auto & sprite : m_sprites.all()) {
        SpriteResource data = sprite.second;
        debug("  ({}) {}x{}, {}, {} on spritesheet {}\n", sprite.first,
              data.m_width, data.m_height,
              data.m_x, data.m_y, data.m_path);
    }

    debug("Loaded textures:\n");
    for (const auto & texture : m_textures) {
        debug("  Path: {}\n", texture.first);
    }
}

ResourceManager::loadPackage(std::string resource_package, PackageType type) {
    Package package(resource_package);
    m_sprites.loadPackage(package, type);
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
