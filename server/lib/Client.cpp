#include "Client.hpp"
#include "format.h"
#include "util.hpp"

#include "json11.hpp"

// Last octet can be the protocol version if we ever decide to care
#define MAGIC_NUMBER "\xCA\xC3\x55\x00"

namespace server {

using namespace json11;

Client::Client(TCPsocket socket)
    : m_logger(stderr, [=] {
          IPaddress *address = SDLNet_TCP_GetPeerAddress(m_socket);
          if (address) {
              return fmt::format("{}: ", *address);
          } else {
              return std::string("[INVALID IP]: ");
          }
      }) {
    m_socket = socket;
    m_state = Pending;
    m_logger.log("Client connected");
}

void Client::checkProtocolVersion() {
    // TODO: This needs timeout logic so that if the magic number is not
    // found after some time then client is also disconnected. Otherwise this
    // would enable rouge clients to perform DoS by holding their sockets open
    // but not actually sending anything.
    if (m_state != Pending) {
        return;
    }
    if (m_buffer.size() < strlen(MAGIC_NUMBER)) {
        return;
    } else {
        char magic[] = MAGIC_NUMBER;
        for (int i = 0; i < strlen(MAGIC_NUMBER); i++) {
            char front = m_buffer.front();
            m_buffer.pop_front();
            if (front != magic[i]) {
                disconnect(fmt::format("Bad magic number at pos {}", i), false);
                return;
            }
        }
        m_state = Connected;
        m_logger.log("Correct magic number (state = Connected)");
    }
}

void Client::exec() {
    if (m_state == Disconnected) {
        return;
    }
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    if (SDLNet_SocketReady(m_socket)) {
        int bytes_recv = SDLNet_TCP_Recv(m_socket,
                                         buffer,
                                         RECV_BUFFER_SIZE - m_buffer.size());
        m_logger.log(fmt::format("Bytes received: {}", bytes_recv));
        if (bytes_recv <= 0) {
            disconnect(
                fmt::format("Left server (recv: {})", bytes_recv), false);
        }
        for (int i = 0; i < bytes_recv; i++) {
            //printf("char recv: %c\n", buffer[i]);
            m_buffer.push_back(buffer[i]);
        }
    }
    checkProtocolVersion();
    if (m_state == Connected) {
        processMessages();
    }
}

void Client::processMessages() {
    int delimi = std::find(m_buffer.begin(),
                           m_buffer.end(), '\0') - m_buffer.begin();
    //fmt::print("delimi: {}\n", delimi);
}

Client::State Client::getState() const { return m_state; }

Client::Client(Client &&other)
    : m_state(other.m_state), m_buffer(std::move(other.m_buffer)),
      m_socket(other.m_socket) {
    other.m_socket = nullptr;
}

Client &Client::operator=(Client &&other) {
    m_state = other.m_state;
    m_buffer = std::move(other.m_buffer);
    m_socket = other.m_socket;
    other.m_socket = nullptr;
    return *this;
}

Client::~Client() { SDLNet_TCP_Close(m_socket); }

TCPsocket Client::getSocket() { return m_socket; }

void Client::disconnect(std::string reason, bool send) {
    m_state = Disconnected;
    m_logger.log("Client disconnected ({})", reason);

    if (!send) {
        return;
    }

    Json json =
        Json::object{ { "type", "disconnect" },
                      { "entity", Json::object{ { "reason", reason } } } };
    std::string str = json.string_value();

    int len = str.size();
    int result = SDLNet_TCP_Send(m_socket, str.c_str(), len);

    // Error sending.
    if (result < len) {
        m_logger.log("SDLNet_TCP_Send: {:s}", SDLNet_GetError());
        m_state = Disconnected;
    }
}
} // namespace server
