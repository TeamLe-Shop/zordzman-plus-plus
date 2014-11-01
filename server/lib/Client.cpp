#include "Client.hpp"
#include "format.h"
#include "util.hpp"

#include "json11.hpp"

#define PROTOCOL_VERSION 0x00
#define MAGIC_NUMBER 0xCAC35500 | PROTOCOL_VERSION

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
    m_buffer.reserve(RECV_BUFFER_SIZE);
    m_logger.log("Client connected");
}

bool Client::checkProtocolVersion() { return true; }

void Client::recv() {
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    int bytes_recv =
        SDLNet_TCP_Recv(m_socket, buffer, RECV_BUFFER_SIZE - m_buffer.size());
    if (bytes_recv <= 0) {
        disconnect("Left server", false);
    } else {
        printf("Message [%d]: %.*s\n", bytes_recv, bytes_recv, buffer);

        if (m_state == Pending) {
            // The client still sometimes gets disconnected
            // because it can't flush and ends up sending a glob
            // of data which contains the magic number and some other
            // garbage.
            // Allowing the initial message to be
            // over 4 bytes should prevent this.
            if (bytes_recv >= 4) {
                int magic_num = MAGIC_NUMBER;
                if (memcmp(buffer, &magic_num, 4) == 0) {
                    m_state = Connected;

                } else {
                    disconnect("Incorrect protocol version/magic number", true);
                }
            } else {
                disconnect("Incorrect protocol version/magic number", true);
            }
        }
        for (int i = 0; i < bytes_recv; i++) {
            m_buffer.push_back(buffer[i]);
        }
    }
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


    Json json = Json::object {
        { "type", "disconnect" },
        { "entity", Json::object { { "reason", reason } } }
    };
    std::string str = json.string_value();

    int len = str.size();
    int result = SDLNet_TCP_Send(m_socket, str.c_str(), len);

    // Error sending.
    if (result < len) {
        fmt::print("SDLNet_TCP_Send: {:s}\n", SDLNet_GetError());
        m_state = Disconnected;
    }
}
} // namespace server
