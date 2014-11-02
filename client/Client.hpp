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

namespace client {
class Client {
public:
    /// @brief Initialize the resources, player and level
    Client(Config const & cfg, HUD hud);
    ~Client();
    /// @brief Game loop.
    void exec();
    /// @brief Get the active game instance
    static Client & get();
    /// @brief Get the window on which things are rendered.
    sys::RenderWindow & getWindow();
    /// @brief Join a game server.
    void joinServer();
    /// @brief Draw the HUD.
    void drawHUD();
    /// @brief Read data from m_socket
    void readData();
    /// @brief Check of the client has the map the server has
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
    Player * m_player;
    Config const & m_cfg;
    HUD m_hud;
};
} // namespace client
