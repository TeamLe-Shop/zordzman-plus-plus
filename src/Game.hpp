#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/System.hpp"
#include "level/Level.hpp"

class Game {
public:
    Game();
    ~Game();
    void exec();

private:
    sys::System m_system;
    sys::RenderWindow m_window;
    Level m_level;
};
