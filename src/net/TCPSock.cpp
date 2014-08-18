#include "net/Net.hpp"

using namespace net;

namespace net {
bool TCPSock::connectToHost(std::string host, int portnum) {
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

void TCPSock::startReading() {
    // Nothing here yet!
}

bool TCPSock::send(std::string buf) {
    if (!m_open)
        return false;
    int len = strlen(buf.c_str()) + 1;
    // Report an error if we sent less bytes than we should have.
    if (SDLNet_TCP_Send(m_socket, (void *)buf.c_str(), len) < len) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    printf("Message: %s", buf.c_str());
    return true;
}

bool TCPSock::send(void *buf, int len) {
    if (!m_open)
        return false;
    // Report an error if we sent less bytes than we should have.
    if (SDLNet_TCP_Send(m_socket, buf, len) < len) {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        return false;
    }

    printf("Message: %s", buf);
    return true;
}

void TCPSock::close() {
    // Check if it's open, and close it if it is.
    if (m_open) {
        SDLNet_TCP_Close(m_socket);
        m_open = false;
    }
}

TCPSock::~TCPSock() {
    // Close the socket.
    close();
}
} // namespace net