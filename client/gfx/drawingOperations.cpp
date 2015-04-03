#include "gfx/drawingOperations.hpp"

#include "Client.hpp"
#include "ResourceManager.hpp"

#include "common/resources/SpriteResource.hpp"

#include <SDL_opengl.h>
#include <string.h>
#include <stdexcept>

#include <SDL_ttf.h>

#include "format.h"

#include "common/util/debug.hpp"

namespace client {
namespace drawingOperations {

sys::Texture const * currentTexture = nullptr;

void drawSprite(std::string name, float x, float y, float w, float h) {
    ResourceManager * manager = &Client::get().m_resources;
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
        currentTexture = 0;
    }

    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void drawText(std::string font, std::string const & text, int x, int y, int w,
              int h) {

    ResourceManager * manager = &Client::get().m_resources;
    FontResource fontresource = manager->m_fonts[font];

    if (!fontresource.m_valid) {
        throw std::runtime_error(fmt::format("Font \"{}\" not found!", font));
        return;
    }

    TTF_Font * ttf_font = manager->getFont(fontresource.m_path);
    SDL_Color col = {255, 255, 255, 255};

    // NOTE: TTF_RenderText_Blended is said to be slower than
    //       TTF_RenderText_Solid (fastest method), however
    //       I can't seem to get TTF_RenderText_Solid it to work on my system,
    //       and I believe it doesn't work well on all Linux systems.
    SDL_Surface * temp_surface = TTF_RenderText_Blended(ttf_font, text.c_str(),
                                                        col);
    if (temp_surface == nullptr) {
        throw std::runtime_error(
            fmt::format("Failed to create text rendering surface using:\n"
                        "  Font (Resource name): \"{}\"\n"
                        "  Text: {}\n"
                        "Error: {}", font, text, TTF_GetError()));
        return;
    }

    // Storing some of the information in these variables because it is freed
    // before we render.
    int width = temp_surface->w;
    int height = temp_surface->h;

    // temp_surface is freed in here!
    sys::TexResult result = sys::Texture::load_texture(temp_surface);


    if (!result.ok) {
        throw std::runtime_error("Failed to convert surface to texture");
        return;
    }

    // We keep binding and unbinding because it is very unlikely
    // that we will draw the exact same text surface again.

    sys::Texture texture(result);
    sys::Texture::bind(texture);

    float right_pos = x + width * (w / fontresource.m_size);
    float bottom_pos = y + height * (h / fontresource.m_size);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(right_pos, y);
    glTexCoord2f(1, 1);
    glVertex2f(right_pos, bottom_pos);
    glTexCoord2f(0, 1);
    glVertex2f(x, bottom_pos);
    glEnd();

    sys::Texture::unbind();
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
