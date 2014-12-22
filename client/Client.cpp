#include "Client.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "weapons/weaponList.hpp"

#include <stdexcept>
#include <format.h>
#include <thread>
#include <dirent.h>
#include <netdb.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <SDL_mixer.h>

#include "json11.hpp"
#include "common/util/stream.hpp"
#include "common/util/fileutil.hpp"
#include "common/util/net.hpp"
#include "common/extlib/hash-library/md5.h"

namespace client {

using namespace json11;

namespace {
Client * game_instance;
std::string const title = "Zordzman v0.0.3";
Mix_Music * music = nullptr;
} // Anonymous namespace

/* Handler functions */
void handler_mapoffer(MessageProcessor<> *processor,
                              MessageEntity entity) {
    fmt::print("lal wot?\n");
}


Client::Client(Config const & cfg, HUD hud)
    : m_window(800, 600, title), m_player(new Player(cfg.name, 0, 0, 1)),
      m_cfg(cfg), m_hud(hud) {
    game_instance = this;

    if ((m_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error(
            fmt::format("Couldn't create socket: {}", strerror(errno)));
    }

    m_socket_addr.sin_family = AF_INET;

    if (!joinServer()) {
        throw std::runtime_error("Couldn't connect to server.");
    }

    m_player->setCombatWeapon(weaponList::zord);
    // Add the player to level.
    m_level.add(m_player);

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

Client::~Client() { close(m_socket); game_instance = nullptr; }

bool Client::joinServer() {
    memset(&m_socket_addr, 0, sizeof(m_socket_addr));

    // Convert human-readable domain name/ip string (m_cfg.host)
    // to `struct sockaddr_in`.
    struct addrinfo *result;
    int error;

    if ((error = getaddrinfo(m_cfg.host.c_str(), NULL, NULL, &result))) {
        fmt::print("Error resolving host name: {}", gai_strerror(error));
        return false;
    }

    memcpy(&m_socket_addr, result->ai_addr, sizeof(struct sockaddr_in));

    m_socket_addr.sin_port = htons(m_cfg.port);

    fmt::print("Server IP: {}\n", common::util::net::ipaddr(m_socket_addr));

    freeaddrinfo(result);

    if (connect(m_socket, (struct sockaddr*)&m_socket_addr, sizeof
        m_socket_addr) < 0) {
        fmt::print(stderr,
                   "[ERROR] Could not connect to host: {}\n",
                   errno, strerror(errno));
        close(m_socket);
        return false;
    }

    fcntl(m_socket, F_SETFL, O_NONBLOCK);

    size_t total_sent = 0;
    size_t additive = 0;
    size_t length = 4;

    while (total_sent < length) {
        additive = send(m_socket, net::MAGIC_NUMBER.c_str(), length, 0);

        if (additive == -1) {
            fmt::print(stderr, "[ERROR] Error sending magic num: {}\n",
                       strerror(errno));
            break;
        }

        total_sent += additive;
    }

    m_msg_proc.setSocket(m_socket);
    m_msg_proc.addHandler("map.offer", handler_mapoffer);

    return true;
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
    m_msg_proc.process();
    m_msg_proc.dispatch();
    // check for disconnection & map-hash?

}

void Client::checkForMap(std::string map, std::string hash) {
    using namespace common::util::file;
    bool found_match = false;

    m_map_name = fileFromPath(map);

    // The client is going to now look for that map file.
    DIR * dir;
    struct dirent * ent;

    if ((dir = opendir("resources/levels/")) == NULL) {
        throw std::runtime_error(
            fmt::format("Couldn't open directory \"{}\"", "resources/levels"));
    }

    while ((ent = readdir(dir)) != NULL) {
        // Does the map hash match the file name?
        if (!strcmp(ent->d_name,
                    hash.c_str())) {
            // Open a stream to the file.
            std::ifstream mapfile(
                fmt::format("resources/levels/{}", ent->d_name),
                std::ios::binary | std::ios::in);

            // Read all data...
            std::vector<char> mapdata =
                common::util::stream::readToEnd(mapfile);

            MD5 md5;
            /// Generate a hash from the map data
            md5.add(mapdata.data(), mapdata.size());
            if (!strcmp(md5.getHash().c_str(), ent->d_name)) {
                found_match = true;
                m_level = Level(hash);
            }

            mapfile.close();
        }
    }

    // Send to the server whether or not we have the map.
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
    std::string serverstr =
        fmt::format("Server: {}", common::util::net::ipaddr(m_socket_addr));
    std::string mapstr = fmt::format("Map: {}", m_map_name);
    drawText(serverstr, 800 - (8 * serverstr.size()), m_hud.border.y - 8, 8, 8);
    drawText(mapstr, 800 - (8 * mapstr.size()), m_hud.border.y - 16, 8, 8);
}

Client & Client::get() {
    if (!game_instance) {
        throw std::runtime_error("Game::get(): Game instance is null.");
    }
    return *game_instance;
}

sys::RenderWindow & Client::getWindow() { return m_window; }


} // namespace client
