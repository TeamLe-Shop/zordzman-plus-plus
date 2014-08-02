#include "drawingOperations.hpp"

#include <cassert>
#include <SDL_opengl.h>

namespace drawingOperations {
void drawSpriteFromSheet(SpriteSheet const &spritesheet, int xOff, int yOff,
                         float x, float y, float w, float h) {
    unsigned int tileSize = spritesheet.getSpriteSize();

    // Correct the tx and ty values.

    float tx_;
    float ty_;

    float ts_ = 0.25f / (float)tileSize;

    // Avoid dividing by zero
    if ((float)(xOff * tileSize) < (ts_)) {
        tx_ = 0;
    } else {
        tx_ = 0.25f / (float)(xOff * tileSize);
    }

    if ((float)(yOff * tileSize) < (ts_)) {
        ty_ = 0;
    } else {
        ty_ = 0.25f / (float)(yOff * tileSize);
    }

    // Bind the spritesheet texture...
    Texture::bind(spritesheet);

    // The meat of the draw() method.
    glTexCoord2f(tx_, ty_);
    glVertex2f(x, y);
    glTexCoord2f(tx_ + ts_, ty_);
    glVertex2f(x + w, y);
    glTexCoord2f(tx_ + ts_, ty_ + ts_);
    glVertex2f(x + w, y + h);
    glTexCoord2f(tx_, ty_ + ts_);
    glVertex2f(x, y + h);

    // unbind the texture.
    Texture::unbind();
}

void drawRectangle(float x, float y, float w, float h, bool filled) {
    if (filled) {
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    } else {
        drawLine(x, y, x + w, y);
        drawLine(x + w, y, x + w, y + h);
        drawLine(x + w, y + h, x, y + h);
        drawLine(x, y + h, x, y);
    }
}

void drawLine(float x1, float y1, float x2, float y2) {
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
}
}
