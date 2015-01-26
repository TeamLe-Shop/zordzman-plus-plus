#include "Client.hpp"
#include "format.h"

#include "common/util/net.hpp"

// Last octet can be the protocol version if we ever decide to care
#define MAGIC_NUMBER "\xCA\xC3\x55\x01"

namespace server {

using namespace json11;

Client::Client(struct sockaddr_in addr, int socket)
    : m_logger(stderr, [=] {
          return fmt::format("{}: ", common::util::net::ipaddr(addr));
      }) {
    m_time_created = time(nullptr);
    m_tcp_socket = socket;
    m_addr = addr;
    m_state = Pending;
    m_channel = -1;
    m_logger.log("Client connected (state = Pending)");
}

void Client::checkProtocolVersion() {
    // TODO: This needs timeout logic so that if the magic number is not
    // found after some time then client is also disconnected. Otherwise this
    // would enable rouge clients to perform DoS by holding their sockets open
    // but not actually sending anything.
    if (m_state != Pending) {
        return;
    }

    if (difftime(time(nullptr), m_time_created) > 5) {
        disconnect("Magic number timeout", true);
    }

    char buffer[4];
    memset(buffer, 0, 4);
    int bytes_recv = recv(m_tcp_socket, buffer, 4 - m_magic_buffer.size(), 0);
    if ((bytes_recv == 0) || (bytes_recv == -1 && errno != EAGAIN)) {
        disconnect("Left server", false);
    }

    m_magic_buffer += buffer;

    if (m_magic_buffer.size() < strlen(MAGIC_NUMBER)) {
        return;
    } else {
        char magic[] = MAGIC_NUMBER;
        for (std::size_t i = 0; i < strlen(MAGIC_NUMBER); i++) {
            char front = m_magic_buffer[i];
            if (front != magic[i]) {
                disconnect(fmt::format("Bad magic number at pos {}", i), false);
                return;
            }
        }
        m_state = Connected;
        m_logger.log("Correct magic number (state = Connected)");
    }
}

void Client::decideClientName(std::vector<Client> const & clients) {
    std::string default_name = "Player";
    std::string new_name = "Player";

    int counter = 0;

    for (int i = 0; i < clients.size(); i++) {
        if (&clients[i] == this) {
            continue;
        }
        if (clients[i].name == new_name) {
            counter++;
            new_name = fmt::format("Player #{}", counter);
            i = 0;
        }
    }

    name = new_name;
}

void Client::exec(Server *server) {
    if (!m_msg_proc.process()) {
        disconnect("User disconnected", false);
    }
    m_msg_proc.dispatch(server, this);
}

Client::State Client::getState() const { return m_state; }

Client::Client(Client &&other)
    : m_tcp_socket(other.m_tcp_socket), m_msg_proc(other.m_msg_proc),
      m_state(other.m_state) {
    other.m_tcp_socket = -1;
}

Client &Client::operator=(Client &&other) {
    m_state = other.m_state;
    m_tcp_socket = other.m_tcp_socket;
    m_msg_proc = other.m_msg_proc;
    other.m_tcp_socket = -1;
    return *this;
}

Client::~Client() {
#ifdef _WIN32
    closesocket(m_tcp_socket);
#else
    close(m_tcp_socket);
#endif
}

void Client::disconnect(std::string reason, bool flush) {
    m_msg_proc.send("disconnect", reason);
    if (flush) {
        m_msg_proc.flushSendQueue();
    }
    m_state = Disconnected;
    m_logger.log("Client disconnected (state = Disconnected): {} ", reason);
}

void Client::disconnect(std::string reason) { disconnect(reason, true); }

void Client::disconnect() { disconnect("You have been disconnected", true); }
} // namespace server
