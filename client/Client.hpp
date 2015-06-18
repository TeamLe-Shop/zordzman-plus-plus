#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/SysContext.hpp"
#include "level/Level.hpp"
#include "Config.hpp"
#include "ResourceManager.hpp"
#include "HUD.hpp"

#include "base64.hpp"
#include "json11.hpp"

#include "common/net/message.hpp"
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

using Processor = net::MessageProcessor<>;

/// The Zordzman client.
namespace client {

struct ChatMessage {
    std::string message;
    uint32_t timestamp;
};

class Client {
public:
    /// Initialize the resources, player and level
    Client(Config const & cfg, HUD hud);
    ~Client();
    /// Game loop.
    void exec();
    /// Get the active game instance
    static Client & get();
    /// Get the window on which things are rendered.
    sys::RenderWindow & getWindow();
    /// Join a game server.
    bool joinServer();
    /// Draw the HUD.
    void drawHUD();
    /// Check if the client has the map the server has
    void checkForMap(std::string map, std::string hash);
    void writeMapContents(std::string contents);
    void addMessage(std::string message);
    int chatSize();
    void input(SDL_Event event);

private:
    Client(const Client &) = delete;
    Client & operator=(const Client &) = delete;
    sys::SysContext m_system;
    sys::RenderWindow m_window;
    net::Socket m_socket;
    struct sockaddr_in m_socket_addr;
    net::MessageProcessor<> m_msg_proc;

    // Handler functions
    void handleMapOffer(Processor * /*processor*/, net::MessageEntity entity);
    void handleMapContents(Processor * /*processor*/,
                           net::MessageEntity entity);
    void handleServerMessage(Processor * /*processor*/,
                             net::MessageEntity entity);
    void handleEntityState(Processor * /*processor*/,
                           net::MessageEntity entity);
    void handlePlayerID(Processor * /*processor*/, net::MessageEntity entity);

    void handlePlayerJoined(Processor *, net::MessageEntity entity);
    void handlePlayerLeft(Processor *, net::MessageEntity entity);
    void handleEntityDeletion(Processor *, net::MessageEntity entity);
    void handleNickTaken(Processor *);
    void handleNickChange(Processor *, net::MessageEntity entity);

    std::vector<ChatMessage> m_chatMessages;
    uint32_t m_lastMessage, m_currentTime;

    bool chat_open = false;
    std::string chat_string;

    // Visual effects for chat
    uint8_t chat_fade_timer;
    uint8_t chat_maxfade = 10;
    uint8_t chat_minfade = 0;

    std::vector<size_t> m_graph_data;
    size_t msgs_recvd;
    size_t max_graph_data = 150;

public:
    ResourceManager m_resources;

private:
    std::string m_map_hash;
    Level m_level;
    std::string m_map_name;
    Config const & m_cfg;
    HUD m_hud;
#ifdef _WIN32
    WSADATA m_wsa_data;
#endif
    Mix_Music * m_music = nullptr;
    unsigned int m_playerID;
    bool m_receivedID = false;
    static Client * m_instance;
};
} // namespace client
