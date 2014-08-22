
#pragma once


#include <string>
#include <SDL_net.h>
#include "format.h"


/// @brief Represents a connected client
///
/// When a message handler is called it is passed the client instance from which
/// the message originated. Therefore this used be used for persisting anything
/// about the client that needs to be passed between subsystems.
class Client {

public:
    Client(TCPsocket socket);
    ~Client();

    /// @brief Convenience logging that includes the peers's address
    ///
    /// Messages are printed to stderr.
    void log(std::string message);

    /// @brief Assert the client is using the correct protocol version
    /// @return True if the correct version, false otherwise
    bool checkProtocolVersion();

private:
    TCPsocket m_socket;
};
