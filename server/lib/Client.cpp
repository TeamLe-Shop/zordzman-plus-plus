
#include "Client.hpp"


Client::Client(TCPSocket socket) {
    m_socket = socket;
}


Client::log(std::string message) {
    IPaddress *address = SDLNet_TCP_GetPeerAddress(m_socket);
    if (!address) {
        // lol what am i meant to do here?
        return;
    }
    fmt::print("{}:{}: {}", address->host, address->port, message);
}
