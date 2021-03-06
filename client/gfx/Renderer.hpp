#pragma once

#include <cstddef>
#include <string>

#include "client/HUD.hpp"
#include "client/gfx/drawingOperations.hpp"
#include "client/level/Level.hpp"
#include "client/sys/RenderWindow.hpp"
#include "common/entity/entity.hpp"

#include "client/gfx/hud/Label.hpp"
#include "client/gfx/hud/Netgraph.hpp"
#include "client/gfx/hud/ChatMessages.hpp"

namespace client {

namespace gfx {

class Renderer {
public:
    Renderer(sys::RenderWindow &window, HUD &hud, Level &level);
    void render();
    void setPlayerID(unsigned int id);
    void clearPlayerID();
    void setServerName(std::string name);
    void setMapName(std::string map);
    void addNetworkData(std::size_t messages_recieved);
    void addMessage(std::string msg);

    bool chat_open = false;
    std::string chat_string;

private:
    sys::RenderWindow &m_window;
    HUD m_hud;
    Level &m_level;
    bool m_have_player_id = false;
    unsigned int m_player_id = 0;
    std::string m_server_name;
    std::string m_map_name;

    // Netgraph
    Netgraph m_netgraph;

    // Chat
    uint8_t chat_fade_timer = 0;
    uint8_t chat_minfade = 0;
    uint8_t chat_maxfade = 10;

    ChatMessages m_chatMessages;

    void drawHUD();
    void drawNetGraph();
    void drawChat();
};

}  // namespace gfx
}  // namespace client
