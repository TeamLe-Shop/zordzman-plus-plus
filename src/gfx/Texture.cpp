#include "Texture.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace {
// Kindly provided by Krootushas.
bool load_image_to_tex(char const *const filename, GLuint &tex) {
    SDL_Surface *surface = IMG_Load(filename);

    if (!surface) {
        return false;
    }

    GLenum texture_format;
    GLint nOfColors;

    // get the number of channels in the SDL surface
    nOfColors = surface->format->BytesPerPixel;

    if (nOfColors == 4) {

        if (surface->format->Rmask == 0x000000ff)
            texture_format = GL_RGBA;

        else
            texture_format = GL_BGRA_EXT;
    } else if (nOfColors == 3) {

        if (surface->format->Rmask == 0x000000ff)

            texture_format = GL_RGB;

        else

            texture_format = GL_BGR_EXT;
    } else
        return false;

    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
    return true;
}
}

void Texture::loadFromFile(std::string const &filename) {
    load_image_to_tex(filename.c_str(), m_handle);
}

int Texture::getWidth() const { return m_width; }

int Texture::getHeight() const { return m_height; }

int Texture::getTileSize() const { return m_tile_size; }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_handle); }

Texture::~Texture() { glDeleteTextures(1, &m_handle); }
