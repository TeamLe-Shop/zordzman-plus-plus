#pragma once

#include "client/gfx/Renderer.hpp"
#include "sys/RenderWindow.hpp"
#include "sys/SysContext.hpp"
#include "level/Level.hpp"
#include "Config.hpp"
#include "ResourceManager.hpp"
#include "HUD.hpp"
#include "net/Client.hpp"
#include "net/messages.hpp"

#include "base64.hpp"
#include "json11.hpp"

#include "common/entity/entity.hpp"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <unistd.h>
#endif

#include <vector>
#include <algorithm>

#include <SDL_mixer.h>
#include <SDL.h>

/// The Zordzman client.
namespace client {

struct ChatMessage {
    std::string message;
    uint32_t timestamp;
};

class Client {
public:
    Client(Config const & cfg, HUD hud);
    ~Client();

    /// Get the active game instance
    static Client & get();

    /// The client's mainloop.
    void exec();

    /// Get the window on which things are rendered.
    sys::RenderWindow & getWindow();

    /// Draw the HUD.
    void drawHUD();

    /// Check if the client has the map the server has
    void checkForMap(std::string map, std::string hash);

    /// Write map to file.
    ///
    /// This creates a file in the configured level directory whose name is
    /// hash of the map being written.
    ///
    /// The map contents is expected to be base64-encoded. It will be decoded
    /// before being written to disk.
    void writeMapContents(std::string contents);

    /// Add a chat message to be displayed.
    void addMessage(std::string message);

private:
    Client(const Client &) = delete;
    Client & operator=(const Client &) = delete;
    bool m_running;

    sys::SysContext m_system;
    sys::RenderWindow m_window;
    gfx::Renderer m_renderer;

    uint32_t m_lastMessage;
    uint32_t m_currentTime;
    std::vector<ChatMessage> m_chatMessages;

    bool chat_open = false;
    std::string chat_string;

    uint8_t chat_fade_timer;
    uint8_t chat_minfade = 0;
    uint8_t chat_maxfade = 10;

    size_t msgs_recvd;
    size_t max_graph_data = 150;
    std::vector<size_t> m_graph_data;

    /// Handle all available SDL events.
    void handleEvents();

public:
    ResourceManager m_resources;

private:
    static Client * m_instance;
    Config const & m_cfg;
    std::string m_map_hash;
    std::string m_map_name;
    Level m_level;
    HUD m_hud;
    Mix_Music * m_music = nullptr;
    unsigned int m_playerID;
    bool m_receivedID = false;
};
} // namespace client
