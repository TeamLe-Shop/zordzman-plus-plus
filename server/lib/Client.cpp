
#include "Client.hpp"
#include "format.h"
#include "util.hpp"


Client::Client(TCPsocket socket) {
    m_socket = socket;
    m_state = PENDING;
    m_buffer_start = m_buffer;
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


bool Client::checkProtocolVersion() {
    return true;
}


void Client::recv() {
    ptrdiff_t buffer_space = m_buffer_start - m_buffer;
    if (SDLNet_SocketReady(m_socket) == 0) {
        return;
    }


    fmt::print(stderr, "{:p} {:p} {}\n",
               (void *) m_buffer, (void *) m_buffer_start, buffer_space);
}
