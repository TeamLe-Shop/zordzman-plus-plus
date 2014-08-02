#include "drawingOperations.hpp"

#include <cassert>
#include <SDL_opengl.h>

namespace drawingOperations {
void drawSpriteFromSheet(SpriteSheet const &spritesheet, int xOff, int yOff,
                         float x, float y, float w, float h) {
    unsigned int const sprSize = spritesheet.getSpriteSize();

    // Calculate the texture coordinates

    float texc_left;
    float texc_top;

    float const texSize = 0.25f / (float)sprSize;

    // Avoid dividing by zero
    if ((float)(xOff * sprSize) < texSize) {
        texc_left = 0;
    } else {
        texc_left = 0.25f / (float)(xOff * sprSize);
    }

    if ((float)(yOff * sprSize) < texSize) {
        texc_top = 0;
    } else {
        texc_top = 0.25f / (float)(yOff * sprSize);
    }

    // Bind the spritesheet texture...
    Texture::bind(spritesheet);

    // The meat of the draw() method.
    glTexCoord2f(texc_left, texc_top);
    glVertex2f(x, y);
    glTexCoord2f(texc_left + texSize, texc_top);
    glVertex2f(x + w, y);
    glTexCoord2f(texc_left + texSize, texc_top + texSize);
    glVertex2f(x + w, y + h);
    glTexCoord2f(texc_left, texc_top + texSize);
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
