#pragma once
#include <string>
#include <vector>

typedef unsigned char byte;

/// @brief A game level
class Level {
public:
    /// @brief Construct the level from a level name
    Level(std::string const &levelname);
    /// @brief Set the width of the level
    void setWidth(int width);
    /// @brief Set the height of the level
    void setHeight(int height);
    /// @brief Get the width of the level
    int getWidth() const;
    /// @brief Get the height of the level
    int getHeight() const;
    /// @brief Get the tile located at (x, y)
    byte tileAt(int x, int y) const;
    /// @brief Set the tile at location (x, y) to t
    void setTileAt(int x, int y, byte t);

private:
    std::vector<byte> m_tiles;
    int m_width, m_height;
};
