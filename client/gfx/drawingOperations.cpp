#include "drawingOperations.hpp"
#include "Client.hpp"

#include "resources/SpriteResource.hpp"

#include <SDL_opengl.h>
#include <string.h>
#include <stdexcept>

namespace client {
namespace drawingOperations {

sys::Texture const * currentTexture = nullptr;
ResourceManager * manager = nullptr;

void setManager(ResourceManager * rmanager) {
    manager = rmanager;
}

void drawSprite(std::string name, float x, float y, float w, float h) {
    if (!manager) {
        throw std::runtime_error("ResourceManager reference is null!");
        return;
    }
    SpriteResource sprite = manager->m_sprites[name];

    drawSpriteFromTexture(manager->getTexture(sprite.m_path.c_str()),
                          sprite.m_x, sprite.m_y, x, y, w, h, sprite.m_width,
                          sprite.m_height);
}

void drawSpriteFromTexture(const sys::Texture & texture, int xOff, int yOff,
                           float x, float y, float w, float h, float sprW,
                           float sprH, SpriteFlip flip) {
    if (xOff < 0 || yOff < 0)
        return;

    // Transform the coordinates to OpenGL texture coordinates
    float const texSpriteW = sprW / texture.getWidth();
    float const texSpriteH = sprH / texture.getHeight();
    float const texc_left = (float) xOff / texture.getWidth();
    float const texc_top = (float) yOff / texture.getHeight();

    // Avoid binding the same texture again, if it was previously bound, as
    // texture binding is an expensive operation
    if (&texture != currentTexture) {
        sys::Texture::bind(texture);
        currentTexture = &texture;
    }
    // Draw a textured quad that represents the sprite
    glBegin(GL_QUADS);
    switch (flip) {
    case SpriteFlip::None:
        glTexCoord2f(texc_left, texc_top);
        glVertex2f(x, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top);
        glVertex2f(x + w, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top + texSpriteH);
        glVertex2f(x + w, y + h);
        glTexCoord2f(texc_left, texc_top + texSpriteH);
        glVertex2f(x, y + h);
        break;
    case SpriteFlip::Horizontal:
        glTexCoord2f(texc_left, texc_top);
        glVertex2f(x + w, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top);
        glVertex2f(x, y);
        glTexCoord2f(texc_left + texSpriteW, texc_top + texSpriteH);
        glVertex2f(x, y + h);
        glTexCoord2f(texc_left, texc_top + texSpriteH);
        glVertex2f(x + w, y + h);
        break;
    case SpriteFlip::Vertical:
        throw std::runtime_error("Unimplemented!");
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
/*sys::Texture const & texture = Client::get().m_resources.getTexture("main");
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
    */
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
