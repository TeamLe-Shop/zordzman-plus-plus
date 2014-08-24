#include "Client.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "gfx/SpriteSheet.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include "format.h"
#include <thread>

namespace client {
namespace {
Client *game_instance;
std::string const title = "Zordzman v0.0.2";
}

Client::Client(Config const &cfg)
    : m_window(800, 600, title), m_level("kek.lvl"),
      m_player(
          new Player("gatsan", m_level.getSpawnX(), m_level.getSpawnY(), 1.5)),
      m_cfg(cfg) {
    game_instance = this;

    // Add the player to level.
    m_level.add(m_player);
}

Client::~Client() { game_instance = nullptr; }

void Client::exec() {
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

        drawUI();

        glColor3f(1, 1, 1);

        m_window.present();
    }
}

void Client::drawUI() {
    using namespace drawingOperations;
    SpriteSheet &sheet = Client::get().resources.getSheet("main");
    auto const width = m_window.getWidth();
    auto const height = m_window.getHeight();

    // Draw the rectangle/box which contains information about the player.
    glColor3f(0.2, 0.2, 0.2);
    drawRectangle(0, 0 + height - 32, width, 32, true);
    glColor3f(0.7, 0.7, 0.7);

    // Format the health string & weapon strings
    auto hptext = fmt::format("HP: {}", m_player->getHealth());

    auto combatwep = m_player->getCombatWeapon().getName();
    bool holdingcombat = m_player->holdingCombatWeapon();
    auto specialwep = m_player->getSpecialWeapon().getName();
    bool holdingspecial = m_player->holdingSpecialWeapon();

    drawText(hptext, 0, 0 + height - 32, 16, 16);
    drawText("WEP:", 0, 0 + height - 32 + 16, 16, 16);

    // Draw the names of the weapons as smaller components

    glColor3f(0.6, 0.6, 0.6);
    if (holdingcombat) {
        glColor3f(0, 1, 0);
    }

    drawText(combatwep, 0 + 64, 0 + height - 32 + 16, 8, 8);

    glColor3f(0.6, 0.6, 0.6);

    if (holdingspecial) {
        glColor3f(0, 1, 0);
    }

    drawText(specialwep, 0 + 64, 0 + height - 32 + 24, 8, 8);

    glColor3f(1, 1, 1);

    drawSpriteFromSheet(sheet, m_player->getCurrentWeapon().x_tile,
                        m_player->getCurrentWeapon().y_tile, 0 + 200,
                        0 + height - 32, 32, 32);

    // Line border to seperate the actual game from the UI
    glColor3f(0, 0, 0.5);
    drawLine(0, 0 + height - 32, 0 + width, 0 + height - 32);
    drawLine(0, 0 + height - 33, 0 + width, 0 + height - 33);
}

Client &Client::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow &Client::getWindow() { return m_window; }
}
