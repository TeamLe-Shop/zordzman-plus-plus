#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/System.hpp"
#include "level/Level.hpp"
#include "net/Net.hpp"
#include "entity/Player.hpp"

class Game {
public:
    /// @brief Initialize the resources, player and level
    Game();
    /// @brief Free resources to avoid memory leaks n shit
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
    net::TCPSock m_socket;
    Level m_level;
    Player *m_player;
};
