#include "TCPSocket.hpp"

#include <format.h>

namespace client {
namespace sys {
using fmt::print;

bool TCPSocket::connectToHost(std::string host, int portnum) {
    // Attempt to resolve the host. Report the error if we couldn't.
    if (SDLNet_ResolveHost(&m_server, host.c_str(), portnum) < 0) {
        print(stderr, "[ERROR] SDLNet_ResolveHost: {}\n", SDLNet_GetError());
        print(stderr, "[ERROR] Could not resolve host {}\n", host);
        return false;
    }

    // Attempt to open the socket to connect to the server.
    // Report the error if we couldn't.
    if (!(m_socket = SDLNet_TCP_Open(&m_server))) {
        print(stderr, "[ERROR] SDLNet_TCP_Open: {}\n", SDLNet_GetError());
        print(stderr, "[ERROR] Could not open socket\n");
        return false;
    }

    close();

    // Set the open flag to true.
    m_open = true;

    m_socketset = SDLNet_AllocSocketSet(1);

    if (SDLNet_TCP_AddSocket(m_socketset, m_socket) < 0) {
        print(stderr, "[ERROR] SDLNet_TCP_AddSocket: {}\n", SDLNet_GetError());
        return false;
    }

    return true;
}

std::string TCPSocket::read() {
    if (!m_open) { return std::string(); }
    if (!SDLNet_CheckSockets(m_socketset, 16)) {
        return std::string();
    } else {
        char buffer[8192];
        memset(buffer, 0, 8192);
        int bytes_recv = SDLNet_TCP_Recv(m_socket, buffer, 8192);

        if (bytes_recv <= 0) {
            close();
            return std::string();
        } else {
            return std::string(buffer);
        }
    }
}

bool TCPSocket::send(std::string buf) {
    return send(static_cast<const void *>(buf.c_str()), buf.size() + 1);
}

bool TCPSocket::send(const void * buf, int len) {
    if (!m_open)
        return false;
    // Report an error if we sent less bytes than we should have.
    if (SDLNet_TCP_Send(m_socket, buf, len) < len) {
        print(stderr, "SDLNet_TCP_Send: {}\n", SDLNet_GetError());
        return false;
    }
    return true;
}

void TCPSocket::close() {
    // Check if it's open, and close it if is.
    if (m_open) {
        SDLNet_TCP_DelSocket(m_socketset, m_socket);
        SDLNet_TCP_Close(m_socket);
        m_open = false;
    }
}

TCPSocket::~TCPSocket() { close(); }
} // namespace net
} // namespace client
