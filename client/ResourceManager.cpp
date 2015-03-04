#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>

#include <format.h>

#include <json11.hpp>

#include "common/star/tarlib.hpp"
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

    json11::Json json;
    json = json11::Json::parse(manifest, error);

    if (!error.empty()) {
        debug("Error while parsing resource package manifest {}!\n",
              resource_package);
    }

    // Should we assume the entire JSON object is an array?
    if (json.is_array()) {
        std::vector<json11::Json> items = json.array_items();

        for (json11::Json j : items) {
            debug("==> {} (type {})\n", j["name"].string_value(),
                  j["type"].string_value());
        }
    } else {
        debug("The manifest is not an array!");
    }

    m_textures.emplace(std::piecewise_construct, std::forward_as_tuple("main"),
                       std::forward_as_tuple("resources/spritesheet.png"));
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
