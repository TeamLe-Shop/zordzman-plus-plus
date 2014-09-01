#include "Client.hpp"
#include "format.h"
#include "util.hpp"

namespace server {
Client::Client(TCPsocket socket) {
    m_socket = socket;
    m_state = PENDING;
    m_buffer.reserve(RECV_BUFFER_SIZE);
    log("Client connected");
}

void Client::log(std::string message) {
    IPaddress *address = SDLNet_TCP_GetPeerAddress(m_socket);
    if (!address) {
        // lol what am i meant to do here?
        return;
    }
    fmt::print("{}: {}\n", IPaddress_AsString(address), message);
}

bool Client::checkProtocolVersion() { return true; }

void Client::recv() {
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    int bytes_recv =
        SDLNet_TCP_Recv(m_socket, buffer, RECV_BUFFER_SIZE - m_buffer.size());
    if (bytes_recv <= 0) {
        m_state = DISCONNECTED;
        log("Client disconnected");
    } else {
        fmt::print("{}\n", buffer);
        for (int i = 0; i < bytes_recv; i++) {
            m_buffer.push_back(buffer[i]);
        }
    }
}
}
