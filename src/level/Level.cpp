#include "Level.hpp"

#include <vector>
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

    bool tileData = false;

    for (size_t i = 0; i < data.size(); i++) {
        if (tileData) {
            map[i] = data[i - 2];
        } else {
            if (i == 0) { // Width
                m_width = data[i];
            } else if (i == 1) { // Height
                m_height = data[i];
                map = new byte[m_width * m_height];
                tileData = true;
            }
        }
    }
}

void Level::setWidth(int width) { m_width = width; }

void Level::setHeight(int height) { m_height = height; }

byte Level::tileAt(int x, int y) { return map[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) { map[x + y * m_width] = tile; }
