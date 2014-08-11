#pragma once

#include <SDL.h>
#include <string>

namespace sys {

class Window {
public:
    Window(unsigned width, unsigned height,
           std::string const title = "Untitled",
           int x = SDL_WINDOWPOS_UNDEFINED, int y = SDL_WINDOWPOS_UNDEFINED,
           Uint32 flags = SDL_WINDOW_OPENGL);
    ~Window();
    Window(Window const &other) = delete;
    Window &operator=(Window const &other) = delete;
    void present();

private:
    SDL_Window *m_handle;
    SDL_GLContext m_glContext;
};
}
