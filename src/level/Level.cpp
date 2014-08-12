#include "Level.hpp"
#include "globalResources.hpp"
#include "gfx/drawingOperations.hpp"
#include "entity/Player.hpp"
#include "Game.hpp"

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
    m_spawnx = data[2] * 32;
    m_spawny = data[3] * 32;
    m_tiles.resize(m_width * m_height);
    std::copy(data.begin() + 4, data.end(), m_tiles.begin());
}

Level::Level(int width, int height, std::vector<byte> tiles)
    : m_width(width), m_height(height), m_tiles(tiles) {}

void Level::setWidth(byte width) { m_width = width; }

void Level::setHeight(byte height) { m_height = height; }

byte Level::getWidth() const { return m_width; }

byte Level::getHeight() const { return m_height; }

int Level::getSpawnX() const { return m_spawnx; }

int Level::getSpawnY() const { return m_spawny; }

byte Level::tileAt(int x, int y) const { return m_tiles[x + y * m_width]; }

void Level::setTileAt(int x, int y, byte tile) {
    m_tiles[x + y * m_width] = tile;
}

void Level::render() const {
    using namespace globalResources;
    using namespace drawingOperations;
    auto &window = Game::get().getWindow();

    int minX = (int)(0 / 32);
    int maxX = minX + window.getWidth() / 32;

    int minY = (int)(0 / 32);
    int maxY = minY + window.getHeight() / 32;

    if (minX < 0)
        minX = 0;
    if (maxX > getWidth() - 1)
        maxX = getWidth() - 1;
    if (minY < 0)
        minY = 0;
    if (maxY > getHeight() - 1)
        maxY = getHeight() - 1;

    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            drawSpriteFromSheet(getSheet("main"), tileAt(x, y), 0, x * 32,
                                y * 32, 32, 32);
        }
    }

    for (auto const &e : entities) {
        e->render();
        e->tick();
    }
}

void Level::add(Entity *e) {
    entities.push_back(std::move(std::unique_ptr<Entity>(e)));
}

Level &Level::operator=(const Level &other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_tiles = other.m_tiles;

    for (auto const &e : other.entities) {
        entities.push_back(std::move(std::unique_ptr<Entity>(e->clone())));
    }

    return *this;
}
