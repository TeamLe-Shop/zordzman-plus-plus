#include "RenderWindow.hpp"

#include <SDL_opengl.h>

namespace sys {

namespace {
void initGL(int width, int height) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 1, -1);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);

    glDisable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}
}

RenderWindow::RenderWindow(unsigned width, unsigned height,
                           std::string const title, int x, int y, unsigned int flags)
    : m_width(width), m_height(height) {
    m_handle = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    m_glContext = SDL_GL_CreateContext(m_handle);
    initGL(width, height);
}

RenderWindow::~RenderWindow() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_handle);
}

void RenderWindow::present() { SDL_GL_SwapWindow(m_handle); }

unsigned RenderWindow::getWidth() const { return m_width; }

unsigned RenderWindow::getHeight() const { return m_height; }
}
