#include "Game.hpp"

#include "Screen.hpp"
#include "globalResources.hpp"
#include "entity/Player.hpp"
#include "gfx/drawingOperations.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

Game::Game()
    : m_window(Screen::WIDTH, Screen::HEIGHT, "Zordzman v0.0.1"),
      m_level("kek.lvl") {
    globalResources::init();
    m_level.add(new Player(300, 300, 1.5));
}

Game::~Game() { globalResources::free(); }

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

        using namespace Screen;
        using namespace drawingOperations;

        glColor3f(0.2, 0.2, 0.2);
        drawRectangle(get_xOffset(), get_yOffset() + HEIGHT - 32, WIDTH, 32,
                      true);
        glColor3f(0.7, 0.7, 0.7);
        drawText("HP: 23", get_xOffset(), get_yOffset() + HEIGHT - 32, 16, 16);
        drawText("WEP:", get_xOffset(), get_yOffset() + HEIGHT - 32 + 16, 16,
                 16);
        glColor3f(0, 1, 0);
        drawText("Zord", get_xOffset() + 64, get_yOffset() + HEIGHT - 32 + 16,
                 8, 8);
        glColor3f(0.6, 0.6, 0.6);
        drawText("Chicken", get_xOffset() + 64,
                 get_yOffset() + HEIGHT - 32 + 24, 8, 8);

        glColor3f(0, 0, 0);
        drawLine(get_xOffset(), get_yOffset() + HEIGHT - 32,
                 get_xOffset() + WIDTH, get_yOffset() + HEIGHT - 32);
        drawLine(get_xOffset(), get_yOffset() + HEIGHT - 33,
                 get_xOffset() + WIDTH, get_yOffset() + HEIGHT - 33);

        glColor3f(1, 1, 1);

        m_window.present();
    }
}
