#include "drawingOperations.hpp"

#include <cassert>
#include <cstddef>
#include <SDL_opengl.h>
#include "globalResources.hpp"
#include <string.h>
#include <iostream>

namespace drawingOperations {

SpriteSheet const *currentSheet = nullptr;

void drawSpriteFromSheet(SpriteSheet const &spritesheet, int xOff, int yOff,
                         float x, float y, float w, float h, char flip) {

    // Transform the coordinates to OpenGL texture coordinates
    float const sprSize = spritesheet.getSpriteSize();
    float const texSpriteW = sprSize / spritesheet.getWidth();
    float const texSpriteH = sprSize / spritesheet.getHeight();
    float const texc_left = texSpriteW * xOff;
    float const texc_top = texSpriteH * yOff;

    if (&spritesheet != currentSheet) {
        sys::Texture::unbind();
        sys::Texture::bind(spritesheet);
        currentSheet = &spritesheet;
    }

    // Draw a textured quad that represents the sprite
    glBegin(GL_QUADS);
    switch (flip) {
    case NO_FLIP:
        glTexCoord2f(texc_left, texc_top);
        glVertex2f(x, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top);
        glVertex2f(x + w, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top + texSpriteH);
        glVertex2f(x + w, y + h);
        glTexCoord2f(texc_left, texc_top + texSpriteH);
        glVertex2f(x, y + h);
        break;
    case X_FLIP:
        glTexCoord2f(texc_left, texc_top);
        glVertex2f(x + w, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top);
        glVertex2f(x, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top + texSpriteH);
        glVertex2f(x, y + h);
        glTexCoord2f(texc_left, texc_top + texSpriteH);
        glVertex2f(x + w, y + h);
        break;
    }
    glEnd();
}

void drawRectangle(float x, float y, float w, float h, bool filled) {
    if (currentSheet != NULL) {
        sys::Texture::unbind();
        currentSheet = NULL;
    }
    if (filled) {

        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
    } else {
        drawLine(x, y, x + w, y);
        drawLine(x + w, y, x + w, y + h);
        drawLine(x + w, y + h, x, y + h);
        drawLine(x, y + h, x, y);
    }
}

void drawLine(float x1, float y1, float x2, float y2) {
    if (currentSheet != NULL) {
        sys::Texture::unbind();
        currentSheet = NULL;
    }
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawText(std::string const &text, int x, int y, int w, int h) {
    SpriteSheet const &sheet = globalResources::getSheet("main");
    for (char c : text) {
        char const *const chars = "abcdefghijklmnopqrstuvwxyz      "
                                  "                                "
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ      "
                                  "0123456789.,:;'\"!?$%()-=+/*_    ";

        char const *char_index = strchr(chars, c);
        if (char_index) {
            ptrdiff_t index = char_index - chars;
            drawSpriteFromSheet(sheet, (index % 32), 26 + (index / 32), x, y, w,
                                h);
            x += w;
        }
    }
}

} // My little namespaaaaaace
