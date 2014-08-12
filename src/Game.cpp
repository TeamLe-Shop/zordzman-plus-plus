#include "Game.hpp"

#include "globalResources.hpp"
#include "entity/Player.hpp"
#include "gfx/drawingOperations.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdexcept>

namespace {
Game *game_instance;
}

Game::Game() : m_window(800, 600, "Zordzman v0.0.1"), m_level("kek.lvl") {
    game_instance = this;
    globalResources::init();
    m_level.add(new Player(300, 300, 1.5));
}

Game::~Game() {
    globalResources::free();
    game_instance = nullptr;
}

void Game::exec() {
    for (;;) {
        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
            continue;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        m_level.render();

        using namespace drawingOperations;
        auto const width = m_window.getWidth();
        auto const height = m_window.getHeight();

        glColor3f(0.2, 0.2, 0.2);
        drawRectangle(0, 0 + height - 32, width, 32, true);
        glColor3f(0.7, 0.7, 0.7);
        drawText("HP: 23", 0, 0 + height - 32, 16, 16);
        drawText("WEP:", 0, 0 + height - 32 + 16, 16, 16);
        glColor3f(0, 1, 0);
        drawText("Zord", 0 + 64, 0 + height - 32 + 16, 8, 8);
        glColor3f(0.6, 0.6, 0.6);
        drawText("Chicken", 0 + 64, 0 + height - 32 + 24, 8, 8);

        glColor3f(0, 0, 0);
        drawLine(0, 0 + height - 32, 0 + width, 0 + height - 32);
        drawLine(0, 0 + height - 33, 0 + width, 0 + height - 33);

        glColor3f(1, 1, 1);

        m_window.present();
    }
}

Game &Game::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow &Game::getWindow() { return m_window; }
