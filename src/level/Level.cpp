#include "Level.hpp"

Level::Level(std::string const level) {
    std::streampos size;
    char *memblock;
    std::ifstream file(("resources/levels/" + level).c_str(),
                       std::ios::in | std::ios::binary | std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "[ERROR] Couldn't open level file resources/levels/%s" <<
               level.c_str());
    }

    // Right now, we're just taking in some basic information about the map.

    size = file.tellg();
    memblock = new char[size];
    file.seekg(0, std::ios::beg);
    file.read(memblock, size);
    file.close();

    bool tileData = false;

    for (int i = 0; i < size; i++) {
        if (tileData) {
            map[i] = memblock[i - 2];
        } else {
            if (i == 0) { // Width
                m_width = memblock[i];
            } else if (i == 1) { // Height
                m_height = memblock[i];
                map = new byte[m_width * m_height];
                tileData = true;
            }
        }
    }

    delete[] memblock;
}

void Level::setWidth(int width) { m_width = width; }

void Level::setHeight(int height) { m_height = height; }

byte Level::tileAt(int x, int y) { return map[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) { map[x + y * m_width] = tile; }
