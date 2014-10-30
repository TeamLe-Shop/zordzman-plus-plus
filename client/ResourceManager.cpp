#include "ResourceManager.hpp"

#include <string>
#include <stdexcept>
#include <tuple>
#include <format.h>

namespace client {
ResourceManager::ResourceManager() {
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
