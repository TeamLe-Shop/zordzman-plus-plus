#pragma once

#include <vector>
#include <string>

namespace server {

namespace map {

typedef unsigned char byte;

class Level {
public:
    /// @brief MD5 hash for the level.
    std::string hash;
    /// @brief Load a level.
    void load_level(std::string map);

private:
    byte m_width, m_height;
    unsigned int m_spawn_x, m_spawn_y;
    std::vector<byte> m_tiles;
};

/// @brief Generate hash from a map
std::string map_hash(std::string map_name);

} // namespace map

} // namespace server
