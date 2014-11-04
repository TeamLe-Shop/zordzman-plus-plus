#pragma once

#include <SDL_net.h>
#include <string>

#define REQUIRES(...) typename std::enable_if<(__VA_ARGS__), int>::type = 0

namespace client {
namespace sys {

/// @brief A TCPSocket object one can use to send and receive data.
class TCPSocket {
public:
    /// @brief Connect to a host.
    ///
    /// @param host The host name of the server.
    /// @param portnum The port number.
    ///
    /// @return Whether or not connecting to the host was successful.
    bool connectToHost(std::string host, int portnum);
    /// @brief Receive data from the host.
    ///
    /// @return Received data.
    std::string read();
    /// @brief Send a string to the host.
    ///
    /// @param buf The string to send.
    ///
    /// @return If sending the data was successful.
    bool send(std::string buf);
    /// @brief Send data to the host.
    ///
    /// @param The pointer to the data.
    /// @param The amount of bytes to send.
    ///
    /// @return If the sending was successful.
    bool send(const void * buf, int len);
    /// @brief Send numeric data to the host
    ///
    /// @param data - The number to send
    ///
    /// return If the sending was successful.
    template <typename T, REQUIRES(std::is_arithmetic<T>())>
    bool send(T const & data) {
        return send(&data, sizeof(T));
    }
    /// @brief Close the socket.
    void close();
    /// @brief Close the socket when destroyed.
    ~TCPSocket();
    /// @brief Get the IP of the server
    IPaddress getServerAddress();
    /// @brief Return IP address of server formatted
    std::string getFormattedServerAddr();

private:
    // The address of the server.
    IPaddress m_server;
    // The actual socket.
    TCPsocket m_socket;
    // Whether it is open or not.
    bool m_open = false;
    // Socket set. This will only contain one socket.
    // The purpose of this is so we can check for activity and avoid
    // waiting for a message and pausing the game.

    // dont blame me blame sdlnet's accent
    SDLNet_SocketSet m_socketset;
};

} // namespace sys
} // namespace client
