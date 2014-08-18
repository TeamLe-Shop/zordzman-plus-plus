#include "Game.hpp"

#include "globalResources.hpp"
#include "entity/Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "net/Net.hpp"


#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>

namespace {
Game *game_instance;
Player *player;
std::string const title = "Zordzman v0.0.1";
}

Game::Game() : m_window(800, 600, title), m_level("kek.lvl") {
    game_instance = this;
    // Initialize the global resources, so we can access various shit like
    // spritesheets and sounds
    globalResources::init();
    // Spawn the player at the level spawn, with a speed of 1.5
    player = new Player(m_level.getSpawnX(), m_level.getSpawnY(), 1.5);
    // Add the player to level.
    m_level.add(player);

    // Initialize SDLNet, so we can use sockets.
    // If it fails, exit with exit code 1.
    if (!net::initNet()) exit(1);

    // Create a new TCP Socket object so we can send shit
    net::TCPSock socket;
    // Try and connect to host "localhost", the the PORT_NUMBER.
    socket.connectToHost("localhost", PORT_NUMBER);
    // Send some data B)
    socket.send("VERSION 0.0.1");
    socket.send("END");

    // Quit SDLNet and clean up. It closes the sockets for us n shit
    net::cleanUp();
}

Game::~Game() {
    // Free resources n shit, so we don't get a memory leak.
    globalResources::free();
    game_instance = nullptr;
}

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

        // The string that contains information about the player's current
        // health.
        char hp_str[8];

        // Use sprintf to put the "HP: <health>" string into hp_str
        sprintf(hp_str, "HP: %d", player->getHealth());

        // Draw the rectangle/box which contains information about the player.
        glColor3f(0.2, 0.2, 0.2);
        drawRectangle(0, 0 + height - 32, width, 32, true);
        glColor3f(0.7, 0.7, 0.7);

        // Draw the health string.
        drawText(hp_str, 0, 0 + height - 32, 16, 16);
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
