#include "Client.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "weapons/weaponList.hpp"
#include "entity/Eyenado.hpp"

#include <stdexcept>
#include <format.h>
#include <thread>
#include <dirent.h>

#include <SDL_mixer.h>

#include "json11.hpp"
#include "common/util/stream.hpp"
#include "common/extlib/hash-library/md5.h"

namespace client {

using namespace json11;

namespace {
Client * game_instance;
std::string const title = "Zordzman v0.0.2";
Mix_Music * music = nullptr;
} // Anonymous namespace

Client::Client(Config const & cfg, HUD hud)
    : m_window(800, 600, title), m_player(new Player("gatsan", 0, 0, 1)),
      m_cfg(cfg), m_hud(hud) {
    game_instance = this;

    joinServer();

    m_player->setCombatWeapon(weaponList::zord);
    // Add the player to level.
    m_level.add(m_player);

    // music n shit
    music = Mix_LoadMUS("resources/music/soundtrack/Lively.ogg");

    if (music == nullptr) {
        throw std::runtime_error(
            fmt::format("Couldn't load sound \"{}\", ({})",
                        "resources/music/soundtrack/Lively.ogg",
                        std::string(Mix_GetError())));
        exit(1);
    }

    // Infinitely loop the music
    Mix_PlayMusic(music, -1);
}

Client::~Client() { game_instance = nullptr; }

void Client::joinServer() {
    m_socket.connectToHost(m_cfg.host, m_cfg.port);
    m_socket.send(&net::MAGIC_NUMBER, 4); // Hand shake
}

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

        // Render the level's tiles and entities
        m_level.render();

        drawHUD();

        glColor3f(1, 1, 1);

        m_window.present();

        readData();

        SDL_Delay(1000 / 60);
    }
}

void Client::readData() {
    // Read shit from socket
    std::string data = m_socket.read();
    data += "\0";
    if (data.size() < 1) { // The connection may have ended or
                           // an error may have occured.
        return;
    }

    std::string err;

    Json json = Json::parse(data, err); // Turn it into a Json object
                                        // so we can access members

    if (!err.empty()) {
        printf("Server sent bad JSON string\n");
        printf("Error: %s\n", err.c_str());
        return;
    }

    if (json["type"].string_value() == "disconnect") {
        printf("Disconnected: %s\n",
               json["entity"]["reason"].string_value().c_str());
    } else if (json["type"].string_value() == "map-hash") {
        checkForMap(json);
    }
}

void Client::checkForMap(Json json) {
    bool found_match = false;

    // The client is going to now look for that map file.
    DIR * dir;
    struct dirent * ent;

    if ((dir = opendir("resources/levels/")) == NULL) {
        throw std::runtime_error(fmt::format(
            "Couldn't open directory \"{}\"", "resources/levels"));
    }

    while ((ent = readdir(dir)) != NULL) {
        // Does the map hash match the file name?
        if (!strcmp(ent->d_name,
                    json["entity"]["hash"].string_value().c_str())) {

            // Open a stream to the file.
            std::ifstream mapfile(fmt::format("resources/levels/{}",
                                  ent->d_name),
                                  std::ios::binary | std::ios::in);

            // Read all data...
            std::vector<char> mapdata =
            common::util::stream::readToEnd(mapfile);

            MD5 md5;
            /// Generate a hash from the map data
            md5.add(mapdata.data(), mapdata.size());
            if (!strcmp(md5.getHash().c_str(), ent->d_name)) {
                found_match = true;
            }

            mapfile.close();
        }
    }

    // Send to the server whether or not we have the map.
    Json hasmap = Json::object {
        { "type", "has-map" },
        { "entity",
        Json::object {
            { "has-map", found_match }
        }
        }
    };
    m_socket.send(hasmap.dump());
}

void Client::drawHUD() {
    using namespace drawingOperations;
    sys::Texture & texture = Client::get().resources.getTexture("main");
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

    setColor(1, 1, 1, 1);

    drawSpriteFromTexture(texture, m_player->getCurrentWeapon()->x_tile,
                          m_player->getCurrentWeapon()->y_tile, 0 + 140,
                          0 + height - 32, 32, 32, 8);

    // Line border to seperate the actual game from the HUD
    setColor(m_hud.border.color);
    drawRectangle(m_hud.border.x, m_hud.border.y, m_hud.border.width,
                  m_hud.border.height);

    glColor3f(1, 1, 1);
    std::string serverstr = fmt::format("Server: {}",
                                        m_socket.getFormattedServerAddr());
    drawText(serverstr,
             800 - (8 * serverstr.size()),
             m_hud.border.y - 8, 8, 8);
}

Client & Client::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow & Client::getWindow() { return m_window; }
} // namespace client
