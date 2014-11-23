#pragma once

#include <SDL.h>
#include <string>

namespace client {
namespace sys {

/// A window that provides a rendering context
class RenderWindow {
public:
    /// Initialize the window.
    ///
    /// @param width The width of the window.
    /// @param height The height of the window.
    /// @param The title of the window, by default "Untitled".
    /// @param The x position of the window on the screen, by default centered.
    /// @param The y position of the window on the screen, by default centered.
    RenderWindow(unsigned width, unsigned height,
                 std::string const title = "Untitled",
                 int x = SDL_WINDOWPOS_UNDEFINED,
                 int y = SDL_WINDOWPOS_UNDEFINED,
                 unsigned int flags = SDL_WINDOW_OPENGL);
    /// Destroy the window
    ~RenderWindow();
    RenderWindow(RenderWindow const & other) = delete;
    /// Assign a window to another
    RenderWindow & operator=(RenderWindow const & other) = delete;
    /// Show the window's contents.
    void present();
    /// Return the width of the window.
    unsigned getWidth() const;
    /// Return the height of the window.
    unsigned getHeight() const;

private:
    SDL_Window * m_handle;
    SDL_GLContext m_glContext;
    unsigned m_width, m_height;
};
} // namespace sys
} // namespace client
