#pragma once

#include "sys/Window.hpp"
#include "sys/System.hpp"
#include "level/Level.hpp"

class Game {
public:
    Game();
    ~Game();
    void exec();

private:
    sys::System m_system;
    sys::Window m_window;
    Level m_level;
};
