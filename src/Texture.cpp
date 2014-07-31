#include "Texture.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <cassert>

void Texture::loadFromFile(std::string const &filename) {
    SDL_Surface *surface = IMG_Load(filename.c_str());

    if (!surface) {
        throw std::runtime_error("Failed to load image");
    }

    m_width = surface->w;
    m_height = surface->h;

    // Build Texture
    glGenTextures(1, &m_handle);
    glBindTexture(GL_TEXTURE_2D, m_handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // TODO: We assume that the pixel data is always in RGBA format.
    // It might not always be the case.
    // We could examine the SDL_Surface for the format of the pixel data.
    assert(surface->format->BitsPerPixel == 32);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
}

int Texture::getWidth() const { return m_width; }

int Texture::getHeight() const { return m_height; }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, m_handle); }

Texture::~Texture() { glDeleteTextures(1, &m_handle); }
