#include "Texture.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include <stdexcept>

#include "common/util/debug.hpp"

using namespace common::util;

namespace client {
namespace sys {

TexResult const TexFail = TexResult{false, 0, 0, 0};

// Kindly provided by Krootushas / 8BitBuff.
TexResult Texture::load_texture(SDL_Surface * surface) {
    if (!surface) {
        debug("Failed to load image: {}\n", std::string(IMG_GetError()));
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
    } else if (bytesPerPixel == 3 || bytesPerPixel == 1) {
        if (surface->format->Rmask == 0x000000ff) {
            texture_format = GL_RGB;
        } else {
            texture_format = GL_BGR_EXT;
        }
    } else {
        debug("Failed to get number of channels in SDL surface\n");
        SDL_FreeSurface(surface);
        return TexFail;
    }

    GLuint tex;

    glGenTextures(1, &tex);

    glBindTexture(GL_TEXTURE_2D, tex);

    // Display each pixel clearly.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel, surface->w, surface->h, 0,
                 texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    TexResult result{true, tex, surface->w, surface->h};
    SDL_FreeSurface(surface);

    // Unbind the texture and return the result.
    glBindTexture(GL_TEXTURE_2D, 0);
    return result;
}
bool Texture::loadFromFile(std::string const & filename) {
    SDL_Surface * surface = IMG_Load(filename.c_str());
    TexResult result = load_texture(surface);
    if (!result.ok) {
        return false;
    }
    m_handle = result.handle;
    m_width = result.width;
    m_height = result.height;
    return true;
}

bool Texture::loadFromMemory(std::string const & memory) {
    SDL_RWops *rwops = SDL_RWFromMem(const_cast<char *>(memory.data()),
                                     memory.size());
    SDL_Surface * surface = IMG_Load_RW(rwops, 1);
    TexResult result = load_texture(surface);
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

void Texture::bind(Texture const & texture) {
    glBindTexture(GL_TEXTURE_2D, texture.m_handle);
}
void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

Texture::~Texture() { glDeleteTextures(1, &m_handle); }

Texture::Texture(std::string const & filename) {
    if (!loadFromFile(filename)) {
        throw std::runtime_error("Failed to construct texture.");
    }
}

Texture::Texture(TexResult result) : m_handle(result.handle),
                                     m_width(result.width),
                                     m_height(result.height) {}

Texture::Texture(std::string const & str, LoadMethod lm) {
    if (lm == File) {
        if (!loadFromFile(str)) {
            throw std::runtime_error("Failed to construct texture.");
        }
    } else if (lm == Memory) {
        if (!loadFromMemory(str)) {
            throw std::runtime_error("Failed to construct texture.");
        }
    }
}

} // namespace sys
} // namespace client
