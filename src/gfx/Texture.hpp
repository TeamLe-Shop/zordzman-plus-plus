#pragma once

#include <string>

#include <SDL_opengl.h>

class Texture {
public:
    void loadFromFile(std::string const &filename);
    std::string getPath() const;
    int getWidth() const;
    int getHeight() const;
    int getTileSize() const;
    void bind() const;
    ~Texture();
    Texture(const Texture &) = delete;
    Texture() = default;

private:
    std::string path;
    GLuint m_handle;
    int m_width, m_height;
    // Tile width & Tile height. by default it's 8.
    int m_tile_size = 8;
};
