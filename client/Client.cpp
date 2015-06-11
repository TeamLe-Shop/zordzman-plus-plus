#include "Client.hpp"

#include "common/entity/entity.hpp"
#include "common/entity/component.hpp"
#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"
#include "common/entity/components/position.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "weapons/weaponList.hpp"

#include "audioPlayer.hpp"
#include "language.hpp"

#include <stdexcept>
#include <format.h>
#include <thread>

#include <stdlib.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <strsafe.h>
#else
#include <dirent.h>
#include <netdb.h>
#include <fcntl.h>
#endif

#include <iostream>

#include "json11.hpp"

#include "common/util/stream.hpp"
#include "common/util/fileutil.hpp"
#include "common/util/net.hpp"
#include "common/util/debug.hpp"
#include "common/util/string.hpp"

#include "common/extlib/hash-library/md5.h"


using namespace net;

namespace client {

using namespace json11;

namespace {
std::string const title = "Zordzman v0.0.3";
bool net_graph = true;
} // Anonymous namespace

Client * Client::m_instance;

namespace {
// Handler functions

void handleDisconnect(Processor * /*processor*/, MessageEntity entity) {
    fmt::print("Disconnected from server ({})\n", entity.string_value());
    // What do I do here? I want to exit, what's the appropriate function to
    // call?
    // TODO: When we implement game states, we should perhaps change this
    // to go back to previous state?
    exit(0);
}

// Systems
void debugSystem(entity::EntityCollection * coll, entity::Entity & ent) {
    auto character = COMPONENT(ent, entity::CharacterComponent);
    auto render    = COMPONENT(ent, entity::RenderComponent);
    auto position  = COMPONENT(ent, entity::PositionComponent);

    auto spriteinfo = render ? render->m_sprite.get() : "[No render component]";
    auto alphainfo = render ? render->m_alpha.get() : 0.f;

     fmt::print("Frame: #{}, Entity ID: #{}:\n"
                "\tCharacter: Name: \"{}\", Health: {}, Max Health: {}\n"
                "\tRender Info: Sprite: \"{}\", Alpha: {:f}\n"
                "\tPosition: ({}, {})\n",
                 coll->getFrame(), ent.getID(), character->m_name.get(),
                 character->m_health.get(), character->m_max_health.get(),
                 spriteinfo, alphainfo,
                 position->m_x.get(),
                 position->m_y.get());
}

}

Client::Client(Config const & cfg, HUD hud)
    : m_window(800, 600, title), m_chatMessages(10),
      m_resources("resources.tar"), m_cfg(cfg), m_hud(hud),
      m_graph_data(150) {

    m_chatMessages.resize(0);
    m_graph_data.resize(0);
#ifdef _WIN32
    WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
    if ((m_socket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        int err = WSAGetLastError();
        throw std::runtime_error(
            fmt::format("Couldn't create socket: (wsagetlasterror: {})", err));
    }
#else
    if ((m_socket = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        throw std::runtime_error(
            fmt::format("Couldn't create socket: {}", strerror(errno)));
    }
#endif

    m_socket_addr.sin_family = AF_INET;

    if (!joinServer()) {
        throw std::runtime_error("Couldn't connect to server.");
    }

    m_level.m_entities.registerComponent(
        entity::CharacterComponent::getComponentName(),
        entity::CharacterComponent::new_);
    m_level.m_entities.registerComponent(
        entity::RenderComponent::getComponentName(),
        entity::RenderComponent::new_);
    m_level.m_entities.registerComponent(
        entity::PositionComponent::getComponentName(),
        entity::PositionComponent::new_);
    m_level.m_entities.addSystem(debugSystem);
    m_instance = this;

    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    audio::playMusic("March");
}

Client::~Client() {
    m_instance = nullptr;
#ifdef _WIN32
    closesocket(m_socket);
    WSACleanup();
#else
    close(m_socket);
#endif
}

bool Client::joinServer() {
    memset(&m_socket_addr, 0, sizeof(m_socket_addr));

    common::util::net::resolvehost(m_socket_addr, m_cfg.host);

    m_socket_addr.sin_port = htons(m_cfg.port);

    fmt::print("Server IP: {}\n", common::util::net::ipaddr(m_socket_addr));

    if (connect(m_socket, (struct sockaddr *)&m_socket_addr,
                sizeof m_socket_addr) < 0) {
        fmt::print(stderr, "[ERROR] ({}) Could not connect to host: {}\n",
                   errno, strerror(errno));
#ifdef _WIN32
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        return false;
    }
#ifdef _WIN32
    ioctlsocket(m_socket, FIONBIO, nullptr);
#else
    fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif

    m_msg_proc.setSocket(m_socket);

    size_t total_sent = 0;
    size_t length = 4;

    while (total_sent < length) {
        auto additive = send(m_socket, net::MAGIC_NUMBER.c_str(), length, 0);

        if (additive == -1) {
            fmt::print(stderr, "[ERROR] Error sending magic num: {}\n",
                       strerror(errno));
            break;
        }

        total_sent += additive;
    }

    using namespace std::placeholders;

    m_msg_proc.addMutedHandler(
        "map.offer", std::bind(&Client::handleMapOffer, this, _1, _2));
    m_msg_proc.addMutedHandler(
        "map.contents", std::bind(&Client::handleMapContents, this, _1, _2));
    m_msg_proc.addMutedHandler(
        "server.message",
        std::bind(&Client::handleServerMessage, this, _1, _2));
    m_msg_proc.addMutedHandler("disconnect", handleDisconnect);
    m_msg_proc.addMutedHandler(
        "entity.state", std::bind(&Client::handleEntityState, this, _1, _2));
    m_msg_proc.addMutedHandler(
        "player.id", std::bind(&Client::handlePlayerID, this, _1, _2));
    m_msg_proc.addMutedHandler(
        "player.joined", std::bind(&Client::handlePlayerJoined, this, _1, _2));
    m_msg_proc.addMutedHandler(
        "player.left", std::bind(&Client::handlePlayerLeft, this, _1, _2));

    m_msg_proc.send("client.nick", m_cfg.name);
    return true;
}

void Client::exec() {
    using namespace drawingOperations;
    for (;;) {
        SDL_Event event;

        // Break from our game loop if they've hit the 'X' button.
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            } else {
                input(event);
            }
        }

        // Clear the screen.
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the level's tiles and entities
        m_level.render();

        m_level.m_entities.cycle();


        drawHUD();

        glColor3f(1, 1, 1);

        m_msg_proc.process(&msgs_recvd);
        m_graph_data.push_back(msgs_recvd);
        if (m_graph_data.size() > max_graph_data) {
            m_graph_data.erase(m_graph_data.begin());
            m_graph_data.resize(max_graph_data);
        }
        m_msg_proc.dispatch();
        m_msg_proc.flushSendQueue();

        // NOTE: Don't depend on SDL_GetTicks too much.
        m_currentTime = SDL_GetTicks();
        if (m_currentTime > m_lastMessage + 5000 && m_chatMessages.size() > 0) {
            std::move(m_chatMessages.begin() + 1, m_chatMessages.end(),
                      m_chatMessages.begin());
            m_chatMessages.resize(m_chatMessages.size() - 1);
            m_lastMessage = m_currentTime;
        }

        m_window.present();
    }
}

void Client::checkForMap(std::string map, std::string hash) {
    using namespace common::util::file;
    bool found_match = false;

    m_map_name = fileFromPath(map);
    m_map_hash = hash;

    // The client is going to now look for that map file.
    auto entries = getDirectoryContents(m_cfg.level_dir);

    for (auto name : entries) {
        // Does the map hash match the file name?
        if (name == hash) {
            // Open a stream to the file.
            std::ifstream mapfile(fmt::format("{}/{}", m_cfg.level_dir, name),
                                  std::ios::binary | std::ios::in);

            // Read all data...
            std::vector<char> mapdata =
                common::util::stream::readToEnd(mapfile);

            MD5 md5;
            // Generate a hash from the map data
            md5.add(mapdata.data(), mapdata.size());
            if (md5.getHash() == name) {
                found_match = true;
                m_level = Level(fmt::format("{}/{}", m_cfg.level_dir, hash));
            } else {
                found_match = false;
            }

            mapfile.close();
        }
    }

    if (!found_match) {
        std::string str = fmt::format("Requesting map: {} ({})", m_map_name,
                                      m_map_hash);
        addMessage(str);
        m_msg_proc.send("map.request", nullptr);
    }
}

void Client::writeMapContents(std::string const map_base64) {
    std::string map_contents = base64_decode(map_base64);
    std::ofstream map_file(fmt::format("{}/{}", m_cfg.level_dir, m_map_hash),
                           std::ios::out | std::ios::binary);
    map_file.write(map_contents.data(), map_contents.size());
    map_file.close();
    m_level = Level(fmt::format("{}/{}", m_cfg.level_dir, m_map_hash));
}

void Client::addMessage(std::string msg) {
    m_lastMessage = SDL_GetTicks();
    if (m_chatMessages.size() == m_chatMessages.capacity()) {
        std::move(m_chatMessages.begin() + 1, m_chatMessages.end(),
                  m_chatMessages.begin());
        m_chatMessages[m_chatMessages.size() - 1] = {msg, m_lastMessage};
    } else {
        m_chatMessages.push_back({msg, m_lastMessage});
    }
}

void Client::handleMapOffer(Processor *, MessageEntity entity) {
    checkForMap(entity["name"].string_value(), entity["hash"].string_value());
}

void Client::handleMapContents(Processor *, MessageEntity entity) {
    writeMapContents(entity.string_value());
}

void Client::handleServerMessage(Processor *, MessageEntity entity) {
    addMessage(fmt::format("{}", entity.string_value()));
}

void Client::handleEntityState(Processor *, MessageEntity entity) {
    m_level.m_entities.handleEntityStateChange(entity);
}

void Client::handlePlayerID(Processor *, MessageEntity entity) {
    if (!entity.is_number()) {
        fmt::print("Server sent invalid player ID! Abandon ship!\n");
        exit(0);
    }
    m_playerID = entity.int_value();
    m_receivedID = true;
}

void Client::handlePlayerJoined(Processor *, MessageEntity entity) {
    audio::playSound("playerjoined");
    addMessage(fmt::format("Player \"{}\" joined the game.",
                           entity.string_value()));
}

void Client::handlePlayerLeft(Processor *, MessageEntity entity) {
    audio::playSound("playerleft");
    addMessage(fmt::format("Player \"{}\" left the game.",
                           entity.string_value()));
}

void Client::drawHUD() {
    using namespace drawingOperations;
    auto const height = m_window.getHeight();
    auto const width = m_window.getWidth();

    if (net_graph) {
        auto const height = m_window.getHeight();
        auto const width = m_window.getWidth();
        glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
        drawRectangle(width - max_graph_data, height - 32 - 100, max_graph_data,
                      100, true);
        glColor4f(0, 0, 1, 0.9f);
        for (size_t i = 0; i < m_graph_data.size(); i++) {
            if (m_graph_data[i]) {
                drawLine(width - m_graph_data.size() + i, height - 32,
                         width - m_graph_data.size() + i,
                         height - 32 - m_graph_data[i] * 2);
            }
        }
        glColor4f(1, 1, 1, 1);
    }

    if (!m_receivedID) {
        return;
    }

    // Draw the rectangle/box which contains information about the player.
    setColor(m_hud.hud_box.color);
    drawRectangle(m_hud.hud_box.x, m_hud.hud_box.y, m_hud.hud_box.width,
                  m_hud.hud_box.height, true);
    setColor(m_hud.font_color);

    // Format the health string & weapon strings

    entity::Entity & player = m_level.m_entities.get(m_playerID);
    auto character = COMPONENT(player, entity::CharacterComponent);
    drawText("default", fmt::format("{}: {}", language::translate("Health"),
             character->m_health.get()), 0,
             0 + height - 32, 16, 16);
    drawText("default", fmt::format("{}:", language::translate("Weapon")),
             0, 0 + height - 32 + 16, 16, 16);

    // Draw the names of the weapons as smaller components

    // Line border to seperate the actual game from the HUD
    setColor(m_hud.border.color);
    drawRectangle(m_hud.border.x, m_hud.border.y, m_hud.border.width,
                  m_hud.border.height);

    glColor3f(1, 1, 1);
    std::string serverstr =
        fmt::format("Server: {}", common::util::net::ipaddr(m_socket_addr));
    std::string mapstr = fmt::format("Map: {}", m_map_name);
    drawText("default", serverstr, width - (8 * serverstr.size()),
             height - 8, 8, 8);
    drawText("default", mapstr, width - (8 * mapstr.size()), height - 16, 8, 8);

    if (chat_open) {
        if (chat_fade_timer < chat_maxfade) {
            chat_fade_timer++;
        }
    } else {
        if (chat_fade_timer > chat_minfade) {
            chat_fade_timer--;
        }
    }

    if (chat_fade_timer) {
        float fade = (float)chat_fade_timer / (float)chat_maxfade;
        glColor4f(0.3, 0.3, 0.3, fade);
        drawRectangle(0, m_hud.border.y - 9, width, 9, false);
        glColor4f(0.2, 0.2, 0.2, fade);
        drawRectangle(1, m_hud.border.y - 9, width - 1, 8);
        glColor4f(1, 1, 1, fade);
        drawText("default", fmt::format("Say: {}", chat_string),
                 0, m_hud.border.y - 9, 8, 8);
    }

    for (size_t i = 0; i < m_chatMessages.size(); i++) {
        glColor4f(0.2, 0.2, 0.2, 0.3);
        size_t len = mbstowcs(NULL, m_chatMessages[i].message.c_str(), 0);
        drawRectangle(0, i * 8, len * 8, 8);
        glColor3f(1, 1, 1);
        drawText("default", m_chatMessages[i].message, 0, i * 8, 8, 8);
    }
}

Client & Client::get() {
    if (!m_instance) {
        throw std::runtime_error("Client::get(): Instance is null.");
    }
    return *m_instance;
}

sys::RenderWindow & Client::getWindow() { return m_window; }

void Client::input(SDL_Event event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RETURN) {
            if (chat_open) {
                SDL_StopTextInput();
                if (!chat_string.empty()) {
                    m_msg_proc.send("chat.message", chat_string);
                }
            } else {
                SDL_StartTextInput();
            }
            chat_open = !chat_open;
            chat_string = "";
        }

        if (event.key.keysym.sym == SDLK_1) {
            audio::playSound("explosion_1");
        } else if (event.key.keysym.sym == SDLK_2) {
            audio::playSound("woah");
        }

        if (chat_open) {
            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                if (!chat_string.empty()) {
                    common::util::string::utf8_pop_character(chat_string);
                } else {
                    audio::playSound("error");
                }
            }
        }
        break;
    case SDL_TEXTINPUT:
        if (chat_open) {
            chat_string += event.text.text;
        }
        break;
    }
}

} // namespace client
