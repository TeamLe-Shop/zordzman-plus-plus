#include "Level.hpp"

#include <iostream>
#include <fstream>

namespace {
// Read all data from a stream into a vector of char
std::vector<char> readAllFromStream(std::istream &stream) {
    std::vector<char> data;
    auto size = stream.tellg();
    data.resize(size);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), size);
    return data;
}
}

Level::Level(std::string const &levelname) {
    std::ifstream file("resources/levels/" + levelname,
                       std::ios::in | std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Couldn't open level file resources/levels/"
                  << levelname;
        std::abort();
    }

    // Right now, we're just taking in some basic information about the map.
    auto data = readAllFromStream(file);
    file.close();

    m_width = data[0];
    m_height = data[1];
    m_tiles.resize(m_width * m_height);

    for (size_t i = 2; i < data.size(); i++) {
        m_tiles[i - 2] = data[i];
    }
}

void Level::setWidth(int width) { m_width = width; }

void Level::setHeight(int height) { m_height = height; }

byte Level::tileAt(int x, int y) { return m_tiles[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) {
    m_tiles[x + y * m_width] = tile;
}
