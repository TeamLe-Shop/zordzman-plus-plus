#pragma once

#include <string>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

class Texture {
public:
    void loadFromFile(const std::string &filename);
    int getWidth() const;
    int getHeight() const;
    void bind() const;
    ~Texture();

private:
    GLuint m_handle;
    int m_width, m_height;
};
