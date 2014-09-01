#include "Client.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "gfx/SpriteSheet.hpp"
#include "weapons/weaponList.hpp"

#include <stdexcept>
#include <format.h>
#include <thread>

namespace client {
namespace {
Client *game_instance;
std::string const title = "Zordzman v0.0.2";
}

Client::Client(Config const &cfg, HUD hud)
    : m_window(800, 600, title), m_level("kek.lvl"),
      m_player(
          new Player("gatsan", m_level.getSpawnX(), m_level.getSpawnY(), 1.5)),
      m_cfg(cfg), m_hud(hud) {
    game_instance = this;

    m_player->setCombatWeapon(weaponList::zord);
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

        drawHUD();

        glColor3f(1, 1, 1);

        m_window.present();
    }
}

void Client::drawHUD() {
    using namespace drawingOperations;
    SpriteSheet &sheet = Client::get().resources.getSheet("main");
    auto const height = m_window.getHeight();

    // Draw the rectangle/box which contains information about the player.
    setColor(m_hud.hud_box.color);
    drawRectangle(m_hud.hud_box.x, m_hud.hud_box.y, m_hud.hud_box.width,
                  m_hud.hud_box.height, true);
    setColor(m_hud.font_color);

    // Format the health string & weapon strings
    auto hptext = fmt::format("HP: {}", m_player->getHealth());

    auto combatwep = m_player->getCombatWeapon()->getName();
    bool holdingcombat = m_player->holdingCombatWeapon();
    auto specialwep = m_player->getSpecialWeapon()->getName();
    bool holdingspecial = m_player->holdingSpecialWeapon();

    drawText(hptext, 0, 0 + height - 32, 16, 16);
    drawText("WEP:", 0, 0 + height - 32 + 16, 16, 16);

    // Draw the names of the weapons as smaller components

    setColor(m_hud.font_color);
    if (holdingcombat) {
        setColor(m_hud.font_color_active);
    }

    drawText(combatwep, 0 + 64, 0 + height - 32 + 16, 8, 8);

    setColor(m_hud.font_color);

    if (holdingspecial) {
        setColor(m_hud.font_color_active);
    }

    drawText(specialwep, 0 + 64, 0 + height - 32 + 24, 8, 8);

    glColor3f(1, 1, 1);

    drawSpriteFromSheet(sheet, m_player->getCurrentWeapon()->x_tile,
                        m_player->getCurrentWeapon()->y_tile, 0 + 140,
                        0 + height - 32, 32, 32);

    // Line border to seperate the actual game from the HUD
    setColor(m_hud.border.color);
    drawRectangle(m_hud.border.x, m_hud.border.y, m_hud.border.width,
                  m_hud.border.height);
}

Client &Client::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow &Client::getWindow() { return m_window; }
}
