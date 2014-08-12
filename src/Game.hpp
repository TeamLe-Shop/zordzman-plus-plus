#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/System.hpp"
#include "level/Level.hpp"

class Game {
public:
    Game();
    ~Game();
    void exec();
    static Game &get();
    sys::RenderWindow &getWindow();

private:
    sys::System m_system;
    sys::RenderWindow m_window;
    Level m_level;
    static Game *m_instance;
};
