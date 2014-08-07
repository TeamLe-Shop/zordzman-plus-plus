#include "Level.hpp"
#include "globalResources.hpp"
#include "gfx/drawingOperations.hpp"
#include "Screen.hpp"

#include <iostream>
#include <fstream>
#include <algorithm>

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

Level::Level(std::string const levelname) {
    std::ifstream file(("resources/levels/" + levelname),
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
    std::copy(data.begin() + 2, data.end(), m_tiles.begin());
}

Level::Level(int width, int height, std::vector<byte> tiles)
    : m_width(width), m_height(height), m_tiles(tiles) {}

void Level::setWidth(byte width) { m_width = width; }

void Level::setHeight(byte height) { m_height = height; }

byte Level::getWidth() const { return m_width; }

byte Level::getHeight() const { return m_height; }

byte Level::tileAt(int x, int y) const { return m_tiles[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) {
    m_tiles[x + y * m_width] = tile;
}

void Level::render() {
    using namespace globalResources;
    using namespace drawingOperations;
	using namespace Screen;

    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
        	if (x*32 < get_xOffset() - 32
        		|| y*32 < get_yOffset() - 32
        		|| x*32 > get_xOffset() + WIDTH
        		|| y*32 > get_yOffset() + HEIGHT) continue;
            drawSpriteFromSheet(getSheet("main"), tileAt(x, y), 0, x * 32,
                                y * 32, 32, 32);
        }
    }
}

Level Level::operator=(const Level &other) {
    return Level(m_width, m_height, other.m_tiles);
}