#pragma once

#include <json11.hpp>
#include <string>

namespace resources {

class SpriteResource {
public:
    SpriteResource();
    SpriteResource(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                   std::string path);

    static SpriteResource constructFromJson(json11::Json json);
    static std::string getTypeName();

    uint32_t m_x, m_y, m_width, m_height;
    std::string m_path;
    bool m_valid;
};

}
