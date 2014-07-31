#pragma once

#include <string>

#include <SDL_opengl.h>

class Texture {
public:
    void loadFromFile(std::string const &filename);
    int getWidth() const;
    int getHeight() const;
    void bind() const;
    ~Texture();

private:
    GLuint m_handle;
    int m_width, m_height;
};
