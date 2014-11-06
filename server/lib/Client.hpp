#pragma once

#include "common/logger/Logger.hpp"

#include <deque>
#include <SDL_net.h>

#define RECV_BUFFER_SIZE 1024

namespace server {

/// @brief Represents a connected client
///
/// When a message handler is called it is passed the client instance from which
/// the message originated. Therefore this used be used for persisting anything
/// about the client that needs to be passed between subsystems.
class Client {

public:
    enum State {
        /// Connection established but protocol version not asserted
        Pending,
        /// Connected and protocol version checked; normal operation
        Connected,
        /// The socket is, or is in the process of disconnecting
        Disconnected,
    };
    /// @brief Construct a new Client instance
    ///
    /// The client's initial state will be set to PENDING.
    Client(TCPsocket socket);

    // TODO: Rewrite this completely fucking wrong doc string or whatever
    // you call it
    /// @brief Read bytes from the socket into the buffer
    ///
    /// Reads up to RECV_BUFFER_SIZE bytes into the buffer.
    /// Therefore the caller is responsible for calling both
    /// SDLNet_CheckSockets and SDLNet_SocketReady
    /// on the socket set containing the client's socket.
    void exec();

    /// @brief Disconnect for `reason`
    ///
    /// @param reason Reason for disconnection
    /// @param send Whether to inform the client of the reason of disconnection.
    void disconnect(std::string reason, bool send);

    State getState() const;

    // Forbid copying
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    // Move operations
    Client(Client &&);
    Client &operator=(Client &&);

    // Destructor
    ~Client();

    /// @brief Return the socket the server uses to communicate with the client.
    TCPsocket getSocket();

    bool sent_map_hash = false;

private:
    State m_state;
    std::deque<char> m_buffer;
    TCPsocket m_socket;
    common::Logger m_logger;

    /// @brief Assert the client is using the correct protocol version
    /// Checks if the buffer contains the magic number, if so the state is set
    /// to Connected, otherwise it is left as-is.
    void checkProtocolVersion();
    void processMessages();
};
} // namespace server
