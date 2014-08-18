#pragma once

#define PORT_NUMBER 4544


#include <SDL.h>
#include <SDL_net.h>
#include <string>

namespace net {

extern char const PROTOCOL_VERSION;

/// @brief Initialize SDLNet.
bool initNet();

/// @brief Quit SDLNet.
void cleanUp();

// The TCPSocket class. Hurr durr?
class TCPSock {
public:
    /// @brief Connect to a host.
    ///
    /// @param host The host name of the server.
    /// @param portnum The port number.
    ///
    /// @return Whether or not connecting to the host was successful.
    bool connectToHost(std::string host, int portnum);
    /// @brief Receive data from the host.
    void startReading();
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
    bool send(void* buf, int len);
    /// @brief Close the socket.
    void close();
    /// @brief Close the socket when destroyed.
    ~TCPSock();
private:
    // The address of the server.
    IPaddress m_server;
    // The actual socket.
    TCPsocket m_socket;
    // Whether it is open or not.
    bool m_open;
};

} //namespace net