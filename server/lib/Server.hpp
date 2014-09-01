#pragma once

#include "Client.hpp"
#include "common/logger/Logger.hpp"

#include <vector>
#include <SDL_net.h>

#define RECV_BUFFER_SIZE 8192

namespace server {
class Server {

public:
    Server(IPaddress address, unsigned int max_clients);
    ~Server();
    int exec();

private:
    /// @brief Accept all pending connections
    ///
    /// This accept(2)s all pending connections on the listening socket. These
    /// new connections are wrapped in a `Client` and added to the `m_clients`
    /// collection as well has having their socket added to the socket set.
    /// If the max number of clients has been reached the new client will be
    /// disconnected immediately.
    void acceptConnections();

    unsigned int m_max_clients;
    TCPsocket m_socket;
    IPaddress m_address;
    std::vector<Client> m_clients;
    SDLNet_SocketSet m_socket_set;
    common::Logger m_logger;
};
}
