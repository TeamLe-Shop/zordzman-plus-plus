#include "Game.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include "format.h"
#include <thread>

namespace {
Game *game_instance;
std::string const title = "Zordzman v0.0.2";
}

Game::Game(Config const &cfg, HUD hud)
    : m_window(800, 600, title), m_level("kek.lvl"),
      m_player(
          new Player("gatsan", m_level.getSpawnX(), m_level.getSpawnY(), 1.5)),
      m_cfg(cfg), m_hud(hud) {
    game_instance = this;

    // Add the player to level.
    m_level.add(m_player);
}

Game::~Game() { game_instance = nullptr; }

void Game::exec() {
    for (;;) {
        SDL_Event event;

        // Break from our game loop if they've hit the 'X' button.
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the level's tiles and entities n hsit
        m_level.render();

        drawHUD();

        glColor3f(1, 1, 1);

        m_window.present();
    }
}

void Game::drawHUD() {
    using namespace drawingOperations;
    auto const width = m_window.getWidth();
    auto const height = m_window.getHeight();

    // Draw the rectangle HUD box which contains information about the player.
    setColor(m_hud.bg_color);

    drawRectangle(0, 0 + height - 32, width, 32, true);

    glColor4f(0.7, 0.7, 0.7, 1);

    // Draw the health string.
    auto hptext = fmt::format("HP: {}", m_player->getHealth());
    drawText(hptext, 0, 0 + height - 32, 16, 16);
    drawText("WEP:", 0, 0 + height - 32 + 16, 16, 16);
    glColor3f(0, 1, 0);
    // Draw the names of the weapons as smaller components
    drawText("Zord", 0 + 64, 0 + height - 32 + 16, 8, 8);
    glColor3f(0.6, 0.6, 0.6);
    drawText("Chicken", 0 + 64, 0 + height - 32 + 24, 8, 8);

    // Line border to seperate the actual game from the UI
    setColor(m_hud.border_color);
    drawLine(0, 0 + height - 32, 0 + width, 0 + height - 32);
    drawLine(0, 0 + height - 33, 0 + width, 0 + height - 33);
}

Game &Game::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow &Game::getWindow() { return m_window; }
