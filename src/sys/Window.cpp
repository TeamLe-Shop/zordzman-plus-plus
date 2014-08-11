#include "Window.hpp"

namespace sys {

Window::Window(unsigned width, unsigned height, std::string const title, int x,
               int y, Uint32 flags) {
    m_handle = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);
    m_glContext = SDL_GL_CreateContext(m_handle);
}

Window::~Window() {
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_handle);
}

void Window::present() { SDL_GL_SwapWindow(m_handle); }
}
