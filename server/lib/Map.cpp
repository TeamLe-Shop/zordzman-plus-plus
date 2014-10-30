#include "Map.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "common/extlib/hash-library/md5.h"
#include "common/util/fileutil.hpp"

namespace server {

namespace map {

using namespace common::util;

void Level::load_level(std::string map_name) {
    std::ifstream file(map_name,
                         std::ios::in | std::ios::binary | std::ios::ate);
    std::vector<char> data = readAllFromStream(file);

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
    std::ifstream file(map_name,
                         std::ios::in | std::ios::binary | std::ios::ate);
    std::vector<char> data = readAllFromStream(file);
    MD5 md5;
    md5.add(data.data(), data.size());
    return md5.getHash();
}

} // namespace map

} // namespace server
