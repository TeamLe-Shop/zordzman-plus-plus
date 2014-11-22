#include "TCPSocket.hpp"

#include <format.h>
#include <cerrno>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

namespace client {
namespace sys {
using fmt::print;

bool TCPSocket::connectToHost(std::string host, int portnum) {
    if ((m_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        print(stderr, "[ERROR] Could not open socket: {}\n", strerror(errno));
        return false;
    }

    memset(&m_address, 0, sizeof m_address);
    m_address.sin_port = htons(portnum);

    struct addrinfo *result;
    int error;

    if ((error = getaddrinfo(host.c_str(), NULL, NULL, &result))) {
        print(stderr, "[ERROR] Could not resolve domain name: {}\n",
              gai_strerror(error));
        return false;
    }

    if (connect(m_socket, (struct sockaddr*)&m_address, sizeof m_address) < 0) {
        print(stderr, "[ERROR] Could not connect to host: {}\n",
              strerror(errno));
        ::close(m_socket);
        return false;
    }

    fcntl(m_socket, F_SETFL, O_NONBLOCK);

    // Set the open flag to true.
    m_open = true;

    return true;
}

std::string TCPSocket::read() {
    if (!m_open) {
        return std::string();
    }
    char buffer[8192];
    memset(buffer, 0, 8192);
    int bytes_recv = ::read(m_socket, buffer, 8192);

    if (bytes_recv <= 0) {
        close();
        return std::string();
    } else {
        return std::string(buffer);
    }
}

bool TCPSocket::send(std::string buf) {
    return send(static_cast<const void *>(buf.c_str()), buf.size() + 1);
}

bool TCPSocket::send(const void * buf, size_t len) {
    if (!m_open || buf == NULL)
        return false;

    int total_bytes_sent = 0;

    // Keep sending until we've sent all the data.
    while (total_bytes_sent < len) {
        total_bytes_sent += write(m_socket, buf, len);
    }
    return true;
}

sockaddr_in TCPSocket::getServerAddress() { return m_server; }

std::string TCPSocket::getFormattedServerAddr() {
    if (!m_open) {
        return "- Not connected -";
    }

    uint32_t addr = m_address.sin_addr.s_addr;
    return fmt::format("{}.{}.{}.{}\n", addr >> 24 & 0xFF, addr >> 16 & 0xFF,
                       addr >> 8 & 0xFF, addr >> 0 & 0xFF, m_address.sin_port);
}

void TCPSocket::close() {
    // Check if it's open, and close it if is.
    if (m_open) {
        ::close(m_socket);
        m_open = false;
    }
}

TCPSocket::~TCPSocket() { close(); }
} // namespace sys
} // namespace client
