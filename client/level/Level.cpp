#include "Level.hpp"
#include "gfx/drawingOperations.hpp"
#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"
#include "common/entity/components/position.hpp"
#include "Client.hpp"
#include "level/tiles/Tile.hpp"
#include "format.h"
#include "common/util/stream.hpp"

#include <fstream>
#include <algorithm>

namespace client {
namespace {
int ticks = 0;

void renderSystem(entity::EntityCollection * coll, entity::Entity & ent) {
    using namespace drawingOperations;
    auto render    = COMPONENT(ent, entity::RenderComponent);
    auto character = COMPONENT(ent, entity::CharacterComponent);
    auto position  = COMPONENT(ent, entity::PositionComponent);

    auto spriteinfo = render ? render->m_sprite.get() : "";
    auto alphainfo = render ? render->m_alpha.get() : 1.f;

    auto xpos = position ? position->m_x.get() : 0;
    auto ypos = position ? position->m_y.get() : 0;

    glColor4f(1, 1, 1, alphainfo);
    drawSprite(spriteinfo, xpos, ypos, 32, 32);
    glColor4f(1, 1, 1, 1);
}

} // Anonymous namespace

using namespace common::util;

Level::Level(std::string const levelname) {
    std::ifstream file((levelname),
                       std::ios::in | std::ios::binary);

    // Complain if that Level couldn't be found / opened.
    if (!file.is_open()) {
        auto str = fmt::format(
            "[ERROR] Couldn't open level file {}", levelname);
        throw std::runtime_error(str);
    }

    auto data = stream::readToEnd(file);
    file.close();

    // Width, height, spawn X and spawn Y are the first 4 bytes.
    m_width = data[0];
    m_height = data[1];
    m_spawnx = data[2] * 32;
    m_spawny = data[3] * 32;
    // Resize the vector to match the width and height.
    m_tiles.resize(m_width * m_height);
    // Copy over the level data over to the vector.

    // NOTE: Only read m_width * m_height bytes
    // To avoid reading more information than the tiles
    std::copy(data.begin() + 4, data.begin() + 4 + m_width * m_height,
              m_tiles.begin());
    m_entities.addSystem(renderSystem);
}

Level::Level(int width, int height, std::vector<byte> tiles)
    : m_width(width), m_height(height), m_tiles(tiles) {
    m_entities.addSystem(renderSystem);
}

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
    using namespace drawingOperations;
    auto & window = Client::get().getWindow();

    // Borders for the renders.
    int minX = (int)(0 / 32);
    int maxX = minX + window.getWidth() / 32;

    int minY = (int)(0 / 32);
    int maxY = minY + window.getHeight() / 32;

    // Make sure we don't render anything beyond what's on the screen
    // or part of the level.
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
            drawSprite(tile::render(tileAt(x, y), ticks), x * 32, y * 32, 32,
                       32);
        }
    }
    // TODO: Render entities
    ticks++;
}

Level & Level::operator=(const Level & other) {
    m_width = other.m_width;
    m_height = other.m_height;
    m_tiles = other.m_tiles;
    m_entities.addSystem(renderSystem);
    return *this;
}

} // namespace client
