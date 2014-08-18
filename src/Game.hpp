#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/System.hpp"
#include "level/Level.hpp"

class Game {
public:
    /// @brief Initialize the resources, player and level
    Game();
    /// @brief Free all resources
    ~Game();
    /// @brief Game loop.
    void exec();
    /// @brief Get the active game instance
    static Game &get();
    /// @brief Get the window on which things are rendered.
    sys::RenderWindow &getWindow();

private:
    sys::System m_system;
    sys::RenderWindow m_window;
    Level m_level;
};
