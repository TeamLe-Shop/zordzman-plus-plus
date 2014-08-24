#pragma once

#include <initializer_list>
#include <string>

#include <SDL_net.h>

#include "Client.hpp"

#define RECV_BUFFER_SIZE 8192

namespace server {
class Server {

public:
    Server(IPaddress *address);
    ~Server();
    int exec();

    template <typename... Args>
    void log(std::string message, Args... messages) {
        print_messages("SERVER:", message, messages...);
        fmt::print(stderr, "\n");
    }

private:
    void print_messages() {}

    template <typename... Args>
    void print_messages(std::string message, Args... messages) {
        fmt::print(stderr, "{} ", message);
        print_messages(messages...);
    }

    /// @brief Accept all pending connections
    ///
    /// This accept(2)s all pending connections on the listening socket. These
    /// new connections are wrapped in a `Client` and then checked to ensure
    /// they're using the correct protocol version. If the client is using the
    /// correct version then they're added to the `m_clients` collection.
    /// Otherwise the connection is terminated.
    void acceptConnections();

    TCPsocket m_socket;
    IPaddress *m_address;
    std::vector<Client> m_clients;
    char m_recv_buffer[RECV_BUFFER_SIZE];
};
}
