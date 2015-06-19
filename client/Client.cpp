#include "Client.hpp"

#include "common/entity/entity.hpp"
#include "common/entity/component.hpp"
#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "weapons/weaponList.hpp"

#include "audioPlayer.hpp"

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


namespace client {

using namespace json11;

namespace {
std::string const title = "Zordzman v0.0.3";
bool net_graph = true;
} // Anonymous namespace

Client * Client::m_instance;

namespace {

// Systems
void debugSystem(entity::EntityCollection * coll, entity::Entity & ent) {
    auto character = COMPONENT(ent, entity::CharacterComponent);
    auto render    = COMPONENT(ent, entity::RenderComponent);

    auto spriteinfo = render ? render->m_sprite.get() : "[No render component]";
    auto alphainfo = render ? render->m_alpha.get() : 0.f;
}

}

Client::Client(Config const & cfg, HUD hud)
    : m_window(800, 600, title), m_chatMessages(10),
      m_resources("resources.tar"), m_cfg(cfg), m_hud(hud),
      m_renderer(m_window, hud, m_level), m_graph_data(150) {
    m_running = true;
    m_chatMessages.resize(0);
    m_graph_data.resize(0);
    m_level.m_entities.registerComponent(
        entity::CharacterComponent::getComponentName(),
        entity::CharacterComponent::new_);
    m_level.m_entities.registerComponent(
        entity::RenderComponent::getComponentName(),
        entity::RenderComponent::new_);
    m_level.m_entities.addSystem(debugSystem);
    m_level.m_entities.addSystem(m_renderer);
    m_instance = this;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    m_renderer.setServerName("localhost:9001");
    m_renderer.setMapName("zm_test");
}

Client::~Client() {
    m_instance = nullptr;
}


void Client::exec() {
    while (m_running) {
        handleEvents();
        m_renderer.render();
        m_level.m_entities.cycle();  // Calls the rendering system
        m_window.present();

        // TODO: refactor this out
        m_graph_data.push_back(msgs_recvd);
        if (m_graph_data.size() > max_graph_data) {
            m_graph_data.erase(m_graph_data.begin());
            m_graph_data.resize(max_graph_data);
        }

        // TODO: refactor this out
        // NOTE: Don't depend on SDL_GetTicks too much.
        m_currentTime = SDL_GetTicks();
        if (m_currentTime > m_lastMessage + 5000
                && m_chatMessages.size() > 0) {
            std::move(m_chatMessages.begin() + 1, m_chatMessages.end(),
                      m_chatMessages.begin());
            m_chatMessages.resize(m_chatMessages.size() - 1);
            m_lastMessage = m_currentTime;
        }
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
        // TODO: m_msg_proc.send("map.request", nullptr);
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


void Client::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_RETURN) {
                    if (chat_open) {
                        SDL_StopTextInput();
                        if (!chat_string.empty()) {
                            // TODO: m_msg_proc.send(
                            //          "chat.message", chat_string);
                        }
                    } else {
                        SDL_StartTextInput();
                    }
                    chat_open = !chat_open;
                    chat_string = "";
                }
                if (chat_open) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE) {
                        if (!chat_string.empty()) {
                            common::util::string::utf8_pop_character(
                                chat_string);
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
}

} // namespace client
