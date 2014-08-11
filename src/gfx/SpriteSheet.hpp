#pragma once

#include "Texture.hpp"

/// @brief A Texture that acts as a sprite sheet
///
/// SpriteSheet is a texture that can be used as a sprite sheet.
///
/// Every sprite sheet must contain uniformly sized sprites.
/// This property is called the "Sprite size" of the sprite sheet.
/// There must be no gaps between the sprites.
///
class SpriteSheet : public Texture {
public:
    /// @brief Get the current sprite size
    unsigned int getSpriteSize() const;
    void setSpriteSize(unsigned int size);
    /// @brief Construct the spritesheet from a file and a sprite size
    ///
    /// @param filename File that contains the sprite sheet
    /// @param spriteSize The sprite size of the sprite sheet
    SpriteSheet(std::string const &filename, unsigned int spriteSize);

private:
    unsigned int m_spriteSize;
};
