#pragma once

#include "entity/Entity.hpp"

#include <string>
#include <vector>
#include <memory>

namespace client {
typedef unsigned char byte;

/// A game level hurr durr
class Level {
public:
    /// Construct the level from a level name
    Level(std::string const levelname);
    /// Construct a level from a vector of TILES
    Level(int width, int height, std::vector<byte> tiles);
    /// Set the width of the level
    void setWidth(byte width);
    /// Set the height of the level
    void setHeight(byte height);
    /// Get the width of the level
    byte getWidth() const;
    /// Get the height of the level
    byte getHeight() const;
    /// Get the player spawn x location of the level
    int getSpawnX() const;
    /// Get the player spawn y location of the level
    int getSpawnY() const;
    /// Get the tile located at (x, y)
    byte tileAt(int x, int y) const;
    /// Set the tile at location (x, y) to t
    void setTileAt(int x, int y, byte t);
    /// hurrdurr render tiles and entities
    void render() const;
    /// Add an entity to the level
    void add(Entity * e);
    /// Remove an entity
    void remove(Entity * e);
    /// Copy level data
    Level & operator=(const Level & other);
    Level() = default;

private:
    byte m_width = 0, m_height = 0;
    int m_spawnx = 0, m_spawny = 0;
    std::vector<byte> m_tiles;
    std::vector<std::unique_ptr<Entity>> entities;
};
} // namespace client
