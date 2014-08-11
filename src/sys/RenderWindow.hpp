#pragma once

#include <SDL.h>
#include <string>

namespace sys {

/// @brief A window that provides a rendering context
class RenderWindow {
public:
    RenderWindow(unsigned width, unsigned height,
                 std::string const title = "Untitled",
                 int x = SDL_WINDOWPOS_UNDEFINED,
                 int y = SDL_WINDOWPOS_UNDEFINED,
                 Uint32 flags = SDL_WINDOW_OPENGL);
    ~RenderWindow();
    RenderWindow(RenderWindow const &other) = delete;
    RenderWindow &operator=(RenderWindow const &other) = delete;
    void present();

private:
    SDL_Window *m_handle;
    SDL_GLContext m_glContext;
};
}
