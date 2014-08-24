#pragma once

#include <vector>
#include <array>
#include <string>

#include <SDL_net.h>

#include "format.h"

#define RECV_BUFFER_SIZE 8192


namespace server {
enum ClientState {
    PENDING,  /// Connection established but protocol version not asserted
    CONNECTED,   /// Connected and protocol version checked; normal operation
    DISCONNECTED,  /// The socket is, or is in the process of disconnecting
};

/// @brief Represents a connected client
///
/// When a message handler is called it is passed the client instance from which
/// the message originated. Therefore this used be used for persisting anything
/// about the client that needs to be passed between subsystems.
class Client {

public:
    /// @brief Construct a new Client instance
    ///
    /// The client's initial state will be set to PENDING.
    Client(TCPsocket socket);

    /// @brief Convenience logging that includes the peers's address
    ///
    /// Messages are printed to stderr.
    void log(std::string message);

    /// @brief Assert the client is using the correct protocol version
    /// @return True if the correct version, false otherwise
    bool checkProtocolVersion();

    /// @brief Read bytes from the socket into the buffer
    ///
    /// Reads up to RECV_BUFFER_SIZE bytes into the buffer. This calls
    /// SDLNet_ReadySocket and only attempts to read if the socket has data
    /// pending. Therefore the caller is responsible for calling
    /// SDLNet_CheckSockets on the socket set containing the client's socket.
    void recv();

    // lol getters and setters
    ClientState m_state;

private:
    std::vector<char> m_buffer;
    TCPsocket m_socket;
};
}
