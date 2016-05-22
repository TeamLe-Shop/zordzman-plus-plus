#include "Client.hpp"

#include "common/entity/entity.hpp"
#include "common/entity/component.hpp"
#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"
#include "common/entity/components/position.hpp"

#include "state/TitleState.hpp"

#include "gfx/drawingOperations.hpp"
#include "net/net.hpp"
#include "json11.hpp"
#include "weapons/weaponList.hpp"

#include "audioPlayer.hpp"
#include "language.hpp"

#include <stdexcept>
#include <format.h>
#include <functional>
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

using namespace std::placeholders;
using namespace json11;

namespace {
std::string const title = "Zordzman v0.0.9";
unsigned int frame = 0;
const unsigned int max_fps = 60;
bool cap_fps = true;
} // Anonymous namespace

Client * Client::m_instance;

Client::Client(Config const & cfg, HUD hud)
    : m_window(800, 600, title), m_cfg(cfg),
      m_hud(hud) {
    m_running = true;
    m_resources.loadPackage("resources.tar", Base);
    m_level.m_entities.registerComponent(
        entity::CharacterComponent::getComponentName(),
        entity::CharacterComponent::new_);
    m_level.m_entities.registerComponent(
        entity::RenderComponent::getComponentName(),
        entity::RenderComponent::new_);
    m_level.m_entities.registerComponent(
        entity::PositionComponent::getComponentName(),
        entity::PositionComponent::new_);
    m_instance = this;
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
//     //m_client.addHandler(std::bind(&Client::onConnect, this, _1));
//     //m_client.addHandler(std::bind(&Client::onMapOffer, this, _1));
//     //m_client.addHandler(std::bind(&Client::onMapContents, this, _1));
//     //m_client.addHandler(std::bind(&Client::onServerMessage, this, _1));
//     //m_client.addHandler(std::bind(&Client::onEntityState, this, _1));
//     //m_client.addHandler(std::bind(&Client::onPlayerId, this, _1));
//     //m_client.addHandler(std::bind(&Client::onPlayerJoined, this, _1));
//     //m_client.addHandler(std::bind(&Client::onPlayerLeft, this, _1));
//     //m_client.addHandler(std::bind(&Client::onEntityDelete, this, _1));
//     //m_client.addHandler(std::bind(&Client::onNickTaken, this, _1));
//     //m_client.addHandler(std::bind(&Client::onNickChange, this, _1));

    m_state = new TitleState();
}

Client::~Client() {
    m_instance = nullptr;
}


void Client::exec() {
    ////m_client.connect(m_cfg.host, m_cfg.port);
    while (m_running) {
        int frame_start_time = SDL_GetTicks();
        handleEvents();
        //glClear(GL_COLOR_BUFFER_BIT);
        m_state->render(this);
        m_window.present();
        frame++;
        int sleeptime = (1000/max_fps) - (SDL_GetTicks()-frame_start_time);
        if (cap_fps && SDL_GetTicks() - frame_start_time < 1000.0 / max_fps) {
            SDL_Delay(sleeptime);
        }
    }
    ////m_client.disconnect();
}

void Client::onConnect(::net::ingress::zm::client::Connected server) {
    ////m_client.send(::net::egress::ClientNick({m_cfg.name}));
}

void Client::onMapOffer(::net::ingress::MapOffer offer) {
    checkForMap(offer.name, offer.hash);
}

void Client::onMapContents(::net::ingress::MapContents contents) {
    writeMapContents(contents.contents);
}

void Client::onServerMessage(::net::ingress::ServerMessage message) {
}

#define JSON_STATE(value)   Json::object { \
                                {"id", (int) state.id}, \
                                {"component", state.component}, \
                                {"field", state.field}, \
                                {"value", value}, \
                            }

void Client::onEntityState(::net::ingress::EntityState state) {
    Json json_state;
    if (PyLong_CheckExact(state.value)) {
        json_state = JSON_STATE((int) PyLong_AsLong(state.value));
    } else if (PyFloat_CheckExact(state.value)) {
        json_state = JSON_STATE(PyFloat_AsDouble(state.value));
    } else if (PyUnicode_CheckExact(state.value)) {
        json_state = JSON_STATE(PyUnicode_AsUTF8(state.value));
    } else {
        return;
    }
    m_level.m_entities.handleEntityStateChange(json_state);
}

#undef JSON_STATE

void Client::onPlayerId(::net::ingress::PlayerId id) {
}

void Client::onPlayerJoined(::net::ingress::PlayerJoined joiner) {
    audio::playSound("playerjoined");
    std::string str = language::translate(language::Key_PlayerJoined);
}

void Client::onPlayerLeft(::net::ingress::PlayerLeft left)
{
    audio::playSound("playerleft");
    std::string str = language::translate(language::Key_PlayerLeft);
}

void Client::onEntityDelete(::net::ingress::EntityDelete entity)
{
    m_level.m_entities.removeEntity(entity.id);
}

void Client::onNickChange(::net::ingress::NickChange change)
{
    std::string str = language::translate(language::Key_NickChange);
}

void Client::onNickTaken(::net::ingress::NickTaken)
{
    std::string str = language::translate(language::Key_NickTaken);
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
        ////m_client.send(::net::egress::MapRequest());
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
                break;;
            default:
                m_state->input(event);
                break;
        }
    }
}

} // namespace client
