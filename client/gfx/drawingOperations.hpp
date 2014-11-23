#pragma once

#include "sys/Texture.hpp"

namespace client {
/// Various drawing operations
namespace drawingOperations {

enum class SpriteFlip { None, Horizontal, Vertical };

/// Draw a sprite from a Texture
///
/// @param texture The texture to use
/// @param xOff The x offset of the sprite in the texture
/// @param yOff The y offset of the sprite in the texture
/// @param x Horizontal position to draw the sprite at
/// @param y Vertical position to draw the sprite at
/// @param w Width of the projection
/// @param h Height of the projection
///
/// If the projection size defined by (w, h) differs from spriteSize,
/// the sprite will be scaled accordingly.
void drawSpriteFromTexture(sys::Texture const & texture, int xOff, int yOff,
                           float x, float y, float w, float h, float spriteSize,
                           SpriteFlip flip = SpriteFlip::None);

/// Draw a rectangle
///
/// @param x X position to draw the rectangle at
/// @param y Y position to draw the rectangle at
/// @param w Width of the rectangle
/// @param h Height of the rectangle
/// @param filled Whether to draw a filled rectangle or a rectangle outline
void drawRectangle(float x, float y, float w, float h, bool filled = true);

/// Draw a line between two endpoints
///
/// @param x1 Horizontal component of endpoint 1
/// @param y1 Vertical component of endpoint1
/// @param x2 Horizontal component of endpoint 2
/// @param y2 Vertical component of endpoint 2

void drawLine(float x1, float y1, float x2, float y2);

/// Draw text
void drawText(std::string const & text, int x, int y, int w, int h);

/// Set the current color.
///
/// @param r Red value.
/// @param g Green value.
/// @param b Blue value.
/// @param a Alpha value.
void setColor(int r, int g, int b, int a);

/// Set the current color.
///
/// @param col The color.
///            The first byte represents red, second green, third blue and last
///            ///            alpha.
void setColor(uint32_t col);
} // namespace drawingOperations
} // namespace client
