#pragma once

#include "Texture.hpp"

/// @brief A Texture that acts as a spritesheet
///
/// It has a thing called a tile size, which is very
/// important you see.
class SpriteSheet : public Texture {
    using Texture::Texture;

public:
    /// @brief Get the current tile size
    int getTileSize() const;
    /// @brief Get the default tile size. It's 8. Yes.
    static int constexpr getDefaultTileSize() { return 8; }

private:
    int m_tile_size = getDefaultTileSize();
};
