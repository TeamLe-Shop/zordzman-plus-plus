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

using namespace json11;
using namespace net;

/// The Zordzman client.
namespace client {
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

private:
    Client(const Client &) = delete;
    Client & operator=(const Client &) = delete;
    sys::SysContext m_system;
    sys::RenderWindow m_window;
    Socket m_socket;
    struct sockaddr_in m_socket_addr;
    MessageProcessor<> m_msg_proc;

    // Is this breaking the rules?
    typedef struct {
        std::string message;
        uint32_t timestamp;
    } Message;
    std::vector<Message> m_chat;
    uint32_t lastMessage, currentTime;

public:
    ResourceManager resources;
    std::string m_map_hash;

private:
    Level m_level;
    std::string m_map_name;
    Config const & m_cfg;
    HUD m_hud;
#ifdef _WIN32
    WSADATA m_wsa_data;
#endif
};
} // namespace client
