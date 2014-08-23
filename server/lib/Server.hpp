#pragma once


#include <initializer_list>
#include <string>

#include <SDL_net.h>

#include "Client.hpp"


class Server {

public:
    Server(IPaddress *address, unsigned int max_clients);
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
    /// new connections are wrapped in a `Client` and added to the `m_clients`
    /// collection as well has having their socket added to the socket set.
    /// If the max number of clients has been reached the new client will be
    /// disconnected immediately.
    void acceptConnections();

    unsigned int m_max_clients;
    TCPsocket m_socket;
    IPaddress *m_address;
    std::vector<Client> m_clients;
    SDLNet_SocketSet m_socket_set;
};
