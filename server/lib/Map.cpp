#include "Map.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "common/extlib/hash-library/md5.h"

namespace server {

namespace map {

void Level::load_level(std::string map_name) {
    std::vector<char> data = read_map(map_name);

    // Width, height, spawn X and spawn Y are the first 4 bytes.
    m_width = data[0];
    m_height = data[1];
    m_spawn_x = data[2] * 32;
    m_spawn_y = data[3] * 32;
    // Resize the vector to match the width and height.
    m_tiles.resize(m_width * m_height);
    // Copy over the level data over to the vector.

    std::copy(data.begin() + 4, data.begin() + 4 + m_width * m_height,
              m_tiles.begin());
}

/// @brief Generate hash from a map
std::string map_hash(std::string map_name) {
    std::vector<char> data = read_map(map_name);
    MD5 md5;
    md5.add(data.data(), data.size());
    return md5.getHash();
}

std::vector<char> read_map(std::string map_name) {
    std::ifstream stream(map_name,
                       std::ios::in | std::ios::binary | std::ios::ate);
    std::vector<char> data;
    auto size = stream.tellg();
    data.resize(size);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), size);
    stream.close();
    return data;
}

} // namespace map

} // namespace server
