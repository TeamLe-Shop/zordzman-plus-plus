#pragma once

#include <vector>
#include <string>

#include "common/extlib/hash-library/md5.h"
#include "base64.hpp"

namespace server {

namespace map {

typedef unsigned char byte;

class Level {
public:
    /// MD5 hash for the level.
    MD5 md5;

    /// Get the Base64-encoded raw level data
    std::string asBase64();

    /// Load a level.
    void loadLevel(std::string map);

    std::string name;
private:
    byte m_width;
    byte m_height;
    unsigned int m_spawn_x;
    unsigned int m_spawn_y;
    std::vector<byte> m_tiles;
    std::string m_base64;
};

} // namespace map

} // namespace server
