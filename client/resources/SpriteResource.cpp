#include "resources/SpriteResource.hpp"

namespace client {

SpriteResource::SpriteResource() : m_valid(false) {}

SpriteResource::SpriteResource(uint32_t x, uint32_t y, uint32_t width,
                               uint32_t height, std::string path)
                               : m_x(x), m_y(y), m_width(width),
                                 m_height(height), m_path(path), m_valid(true) {
}

SpriteResource SpriteResource::constructFromJson(json11::Json json) {
    return SpriteResource(json["x"].int_value(), json["y"].int_value(),
                          json["width"].int_value(), json["height"].int_value(),
                          json["path"].string_value());
}

std::string SpriteResource::getTypeName() {
    return "sprite";
}

}
