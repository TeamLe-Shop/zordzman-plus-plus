#pragma once

#include "sys/Texture.hpp"

namespace client {
/// @brief Various drawing operations
namespace drawingOperations {

char const NO_FLIP = 0, X_FLIP = 1, Y_FLIP = 2;

/// @brief Draw a sprite from a SpriteSheet
///
/// @param spritesheet The sprite sheet to use
/// @param xOff The x offset of the sprite in the sheet
/// @param yOff The y offset of the sprite in the sheet
/// @param x Horizontal position to draw the sprite at
/// @param y Vertical position to draw the sprite at
/// @param w Width of the projection
/// @param h Height of the projection
///
/// If the projection size differs from the tile size of the sprite sheet,
/// the sprite will be scaled.
void drawSpriteFromTexture(sys::Texture const & texture, int xOff, int yOff,
                           float x, float y, float w, float h, float spriteSize,
                           char flip = NO_FLIP);

/// @brief Draw a rectangle
///
/// @param x X position to draw the rectangle at
/// @param y Y position to draw the rectangle at
/// @param w Width of the rectangle
/// @param h Height of the rectangle
/// @param filled Whether to draw a filled rectangle or a rectangle outline
void drawRectangle(float x, float y, float w, float h, bool filled = true);

/// @brief Draw a line between two endpoints
///
/// @param x1 Horizontal component of endpoint 1
/// @param y1 Vertical component of endpoint1
/// @param x2 Horizontal component of endpoint 2
/// @param y2 Vertical component of endpoint 2

void drawLine(float x1, float y1, float x2, float y2);

/// @brief Draw text
void drawText(std::string const & text, int x, int y, int w, int h);

void setColor(int r, int g, int b, int a);

void setColor(uint32_t col);
}
}
