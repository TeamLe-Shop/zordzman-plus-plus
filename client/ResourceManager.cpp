#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

#include "resources/SpriteResource.hpp"

using namespace common::util;

namespace client {
ResourceManager::ResourceManager(std::string resource_package) {
    Tar tar(resource_package);
    std::string manifest, error;
    for (TarEntry * e : tar.getEntries()) {
        // Look for the manifest.
        if (std::string(e->name) == "manifest") {
            manifest = e->contents;
        }
    }

    if (manifest == std::string()) {
        throw std::runtime_error("Manifest file was not found (or empty)!\n");
    }

    json11::Json json;
    json = json11::Json::parse(manifest, error);

    if (!error.empty()) {
        throw std::runtime_error(
            fmt::format("Error while parsing resource package manifest {}!\n"
                        "JSON error: {}\n",
                        resource_package, error)
        );
    }

    // Should we assume the entire JSON object is an array?
    if (json.is_array()) {
        std::vector<json11::Json> items = json.array_items();

        for (json11::Json j : items) {
            if (j["type"] == "sprite") {
                m_sprites.addFromJson(j);
            }
        }
    } else {
        throw std::runtime_error("The manifest is not an array!");
    }

    debug("Loaded sprites:\n");
    for (auto & sprite : m_sprites.map()) {
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

sys::Texture & ResourceManager::getTexture(char const * const key) {
    auto iter = m_textures.find(key);

    if (iter == m_textures.end()) {
        throw std::runtime_error(
            fmt::format("Couldn't find texture \"{}\"", key));
    }

    return iter->second;
}

} // namespace client
