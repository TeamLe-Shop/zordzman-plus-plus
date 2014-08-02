#include "Texture.hpp"

#include <SDL_image.h>
#include <stdexcept>
#include <iostream>
#include <cassert>

namespace {
struct TexResult {
    bool ok;
    GLuint handle;
    int width, height;
};
TexResult constexpr TexFail = TexResult{ false, 0, 0, 0 };

// Kindly provided by Krootushas.
TexResult load_texture(char const *const filename) {
    SDL_Surface *surface = IMG_Load(filename);

    if (!surface) {
        return TexFail;
    }

    GLenum texture_format;
    GLint bytesPerPixel;

    // get the number of channels in the SDL surface
    bytesPerPixel = surface->format->BytesPerPixel;

    if (bytesPerPixel == 4) {
        if (surface->format->Rmask == 0x000000ff) {
            texture_format = GL_RGBA;
        } else {
            texture_format = GL_BGRA_EXT;
        }
    } else if (bytesPerPixel == 3) {
        if (surface->format->Rmask == 0x000000ff) {
            texture_format = GL_RGB;
        } else {
            texture_format = GL_BGR_EXT;
        }
    } else {
        SDL_FreeSurface(surface);
        return TexFail;
    }

    GLuint tex;

    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_FreeSurface(surface);
    return TexResult{ true, tex, surface->w, surface->h };
}
}

bool Texture::loadFromFile(std::string const &filename) {
    TexResult result = load_texture(filename.c_str());
    if (!result.ok) {
        return false;
    }
    m_handle = result.handle;
    m_width = result.width;
    m_height = result.height;
    return true;
}

int Texture::getWidth() const { return m_width; }

int Texture::getHeight() const { return m_height; }

void Texture::bind(const Texture &texture) {
    glBindTexture(GL_TEXTURE_2D, texture.m_handle);
}
void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

Texture::Texture(const std::string &filename) {
    if (!loadFromFile(filename)) {
        throw std::runtime_error("Failed to construct texture.");
    }
}
