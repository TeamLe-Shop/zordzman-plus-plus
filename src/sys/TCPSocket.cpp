#include "TCPSocket.hpp"

#include <iostream>

namespace net {
bool TCPSocket::connectToHost(std::string host, int portnum) {
    // Attempt to resolve the host. Report the error if we couldn't.
    if (SDLNet_ResolveHost(&m_server, host.c_str(), portnum) < 0) {
        fprintf(stderr, "[ERROR] SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        fprintf(stderr, "[ERROR] Could not resolve host %s\n", host.c_str());
        return false;
    }

    // Attempt to open the socket to connect to the server.
    // Report the error if we couldn't.
    if (!(m_socket = SDLNet_TCP_Open(&m_server))) {
        fprintf(stderr, "[ERROR] SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        fprintf(stderr, "[ERROR] Could not open socket\n");
        return false;
    }
    // Set the open flag to true.
    m_open = true;
    return true;
}

void TCPSocket::startReading() {
    // Nothing here yet!
}

bool TCPSocket::send(std::string buf) {
    return send(static_cast<const void *>(buf.c_str()), buf.size() + 1);
}

bool TCPSocket::send(const void *buf, int len) {
    if (!m_open)
        return false;
    // Report an error if we sent less bytes than we should have.
    if (SDLNet_TCP_Send(m_socket, buf, len) < len) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }
    return true;
}

void TCPSocket::close() {
    // Check if it's open, and close it if it is.
    if (m_open) {
        SDLNet_TCP_Close(m_socket);
        m_open = false;
    }
}

TCPSocket::~TCPSocket() {
    close();
}
} // namespace net
