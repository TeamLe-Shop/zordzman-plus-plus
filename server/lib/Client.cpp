
#include "Client.hpp"

Client::Client(TCPsocket socket) {
    m_socket = socket;
    log("Client connected");
}

void Client::log(std::string message) {
    IPaddress *address = SDLNet_TCP_GetPeerAddress(m_socket);
    if (!address) {
        // lol what am i meant to do here?
        return;
    }
    fmt::print("{}:{}: {}\n", address->host, address->port, message);
}

bool Client::checkProtocolVersion() { return true; }
