#pragma once

#include "sys/RenderWindow.hpp"
#include "sys/SysContext.hpp"
#include "level/Level.hpp"
#include "sys/TCPSocket.hpp"
#include "entity/Player.hpp"
#include "Config.hpp"
#include "ResourceManager.hpp"
#include "HUD.hpp"

#include "json11.hpp"

using namespace json11;

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
    /// Read data from m_socket
    void readData();
    /// Check if the client has the map the server has
    void checkForMap(Json json);

private:
    Client(const Client &) = delete;
    Client & operator=(const Client &) = delete;
    sys::SysContext m_system;
    sys::RenderWindow m_window;
    sys::TCPSocket m_socket;

public:
    ResourceManager resources;

private:
    Level m_level;
    std::string m_map_name;
    Player * m_player;
    Config const & m_cfg;
    HUD m_hud;
};
} // namespace client
