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

Game::Game(Config const &cfg)
    : m_window(800, 600, title), m_level("kek.lvl"),
      m_player(
          new Player("gatsan", m_level.getSpawnX(), m_level.getSpawnY(), 1.5)),
      m_cfg(cfg) {
    game_instance = this;

    // Create a new thread for the server if we're connecting
    // to localhost.
    if (cfg.host == "localhost") {
        std::thread([=] { m_server.exec(); }).detach();
    }
    joinServer(cfg.host);
    // Add the player to level.
    m_level.add(m_player);
}

void Game::joinServer(std::string host) {
    m_socket.connectToHost(host, m_cfg.port);
    m_socket.send(sys::PROTOCOL_VERSION);
    using namespace json11;

    Json credentials = Json::object {
        {"type", "credentials"},
        {"entity", Json::object {
            {"name", m_player->getUsername()}
        }},
    };

    fmt::print("Sending credentials: \"{}\"", credentials.dump());

    m_socket.send(credentials.dump());
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

        using namespace drawingOperations;
        auto const width = m_window.getWidth();
        auto const height = m_window.getHeight();

        // Draw the rectangle/box which contains information about the player.
        glColor3f(0.2, 0.2, 0.2);
        drawRectangle(0, 0 + height - 32, width, 32, true);
        glColor3f(0.7, 0.7, 0.7);

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
        glColor3f(0, 0, 0.5);
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
