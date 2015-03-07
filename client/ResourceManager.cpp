#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include "common/util/debug.hpp"

using namespace common::util;

namespace client {
ResourceManager::ResourceManager() {
    std::string resource_package = "resources.tar";
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
            debug("==> Loading {} (type {})\n", j["name"].string_value(),
                  j["type"].string_value());
            loadSprite(j, tar);
        }
    } else {
        throw std::runtime_error("The manifest is not an array!");
    }

    debug("Loaded sprites:\n");
    for (const auto & sprite : m_spritedata) {
        auto data = sprite.second;
        debug("  ({}) {}x{}, {}, {} on spritesheet {}\n", sprite.first,
              data.width, data.height,
              data.x, data.y, data.path);
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

void ResourceManager::loadSprite(json11::Json json, Tar tar) {
    if (json["type"].string_value() == "sprite") {
        // Ugly.
        std::string sprite_name = json["name"].string_value();
        std::string sprite_path = json["path"].string_value();

        SpriteData data = { static_cast<uint32_t>(json["x"].int_value()),
                            static_cast<uint32_t>(json["y"].int_value()),
                            static_cast<uint32_t>(json["width"].int_value()),
                            static_cast<uint32_t>(json["height"].int_value()),
                            sprite_path };
        m_spritedata.emplace(std::piecewise_construct,
                             std::forward_as_tuple(sprite_name),
                             std::forward_as_tuple(data));

        auto iter = m_textures.find(sprite_path);

        if (iter == m_textures.end()) {
            for (TarEntry * e : tar.getEntries()) {
                if (std::string(e->name) == sprite_path) {
                    m_textures.emplace(std::piecewise_construct,
                               std::forward_as_tuple(sprite_path),
                               std::forward_as_tuple(e->contents, sys::MEMORY));
                }
            }
        }
    }
}
} // namespace client
