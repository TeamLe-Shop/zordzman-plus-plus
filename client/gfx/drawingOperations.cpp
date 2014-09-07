#include "drawingOperations.hpp"
#include "Client.hpp"

#include <SDL_opengl.h>
#include <string.h>

namespace client {
namespace drawingOperations {

sys::Texture const * currentTexture = nullptr;

void drawSpriteFromTexture(const sys::Texture & texture, int xOff, int yOff,
                           float x, float y, float w, float h, float sprSize,
                           char flip) {
    if (xOff < 0 || yOff < 0)
        return;

    // Transform the coordinates to OpenGL texture coordinates
    float const texSpriteW = sprSize / texture.getWidth();
    float const texSpriteH = sprSize / texture.getHeight();
    float const texc_left = texSpriteW * xOff;
    float const texc_top = texSpriteH * yOff;
    // If the spritesheet passed in isn't the currentSheet, bind
    // the spritesheet instead, and set currentSheet to the address
    // of spritesheet.
    if (&texture != currentTexture) {
        sys::Texture::bind(texture);
        currentTexture = &texture;
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
    // Unbind any textures if we have any bound. This will avoid
    // our rectangle being fucked up beyond recgonition.
    if (currentTexture) {
        sys::Texture::unbind();
        currentTexture = nullptr;
    }
    // We can choose between a filled whole rectangle, or just an outline.
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
    if (currentTexture) {
        sys::Texture::unbind();
        currentTexture = nullptr;
    }

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawText(std::string const & text, int x, int y, int w, int h) {
    sys::Texture const & texture = Client::get().resources.getTexture("main");
    for (char c : text) {
        char const * const chars = "abcdefghijklmnopqrstuvwxyz      "
                                   "                                "
                                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ      "
                                   "0123456789.,:;'\"!?$%()-=+/*_    ";
        char const * char_index = strchr(chars, c);
        // If the current character is found in chars...
        if (char_index) {
            ptrdiff_t index = char_index - chars;
            // Find it and draw it.
            drawSpriteFromTexture(texture, (index % 32), 26 + (index / 32), x,
                                  y, w, h, 8);
            x += w;
        }
    }
}

void setColor(int r, int g, int b, int a) {
    glColor4f(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

void setColor(uint32_t col) {
    glColor4f((col >> 24 & 0xFF) / 255.0f, (col >> 16 & 0xFF) / 255.0f,
              (col >> 8 & 0xFF) / 255.0f, (col & 0xFF) / 255.0f);
}

} // namespace drawingOperations
} // namespace client
