#pragma once

#include "Texture.hpp"

/// @brief A Texture that acts as a spritesheet
///
/// It has a thing called a tile size, which is very
/// important you see.
class SpriteSheet : public Texture {
public:
    /// @brief Get the current tile size
    unsigned int getTileSize() const;
    /// @brief Get the default tile size. It's 8. Yes.
    static unsigned int constexpr getDefaultTileSize() { return 8; }
    /// @brief Construct the spritesheet from a file
    SpriteSheet(std::string const &filename);

private:
    unsigned int m_tile_size = getDefaultTileSize();
};
