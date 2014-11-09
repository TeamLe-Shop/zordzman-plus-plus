#pragma once

#include <vector>
#include <string>

#include "common/extlib/hash-library/md5.h"

namespace server {

namespace map {

typedef unsigned char byte;

class Level {
public:
    /// @brief MD5 hash for the level.
    MD5 md5;
    /// @brief Load a level.
    void loadLevel(std::string map);

private:
    byte m_width, m_height;
    unsigned int m_spawn_x, m_spawn_y;
    std::vector<byte> m_tiles;
};

} // namespace map

} // namespace server
