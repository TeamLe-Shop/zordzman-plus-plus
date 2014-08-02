#pragma once

#include "SpriteSheet.hpp"

/// @brief Various drawing operations
namespace drawingOperations {

/// @brief Draw a sprite from a sprite sheet
///
/// @param spritesheet The sprite sheet to use
/// @param tx The x offset of the sprite in the sheet
/// @param ty The y offset of the sprite in the sheet
/// @param x Horizontal position to draw the sprite at
/// @param y Vertical position to draw the sprite at
/// @param w Width of the sprite
/// @param h Height of the sprite
///
/// @warning This function expects the GL drawing mode to be GL_QUADS
void drawSpriteFromSheet(SpriteSheet const &spritesheet, int tx, int ty,
                         float x, float y, float w, float h);

/// @brief Draw a rectangle
///
/// @param x X position to draw the rectangle at
/// @param y Y position to draw the rectangle at
/// @param w Width of the rectangle
/// @param h Height of the rectangle
/// @param filled Whether to draw a filled rectangle or a rectangle outline
///
/// @warning This function expects the GL drawing mode to be
/// - GL_QUADS when drawing a filled rectangle
/// - GL_LINES when drawing a rectangle outline
void drawRectangle(float x, float y, float w, float h, bool filled = true);

/// @brief Draw a line between two endpoints
///
/// @param x1 Horizontal component of endpoint 1
/// @param y1 Vertical component of endpoint1
/// @param x2 Horizontal component of endpoint 2
/// @param y2 Vertical component of endpoint 2
///
/// @warning This function expects the GL drawing mode to be GL_LINES
void drawLine(float x1, float y1, float x2, float y2);
}
