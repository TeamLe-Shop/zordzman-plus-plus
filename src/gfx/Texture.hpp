#pragma once

#include <string>

#include <SDL_opengl.h>

class Texture {
public:
    void loadFromFile(std::string const &filename);
    // Kindly provided by Krootushas.
    bool load_image_to_tex(char const *filename, GLuint *);
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
    int tile_size = 8;
};
