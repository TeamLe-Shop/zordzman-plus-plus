#include "Client.hpp"
#include "format.h"
#include "util.hpp"

#define PROTOCOL_VERSION 0x00
#define MAGIC_NUMBER 0xCAC35500 | PROTOCOL_VERSION

namespace server {
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
        printf("MSG [%d]: %.*s\n\n", bytes_recv, bytes_recv, buffer);

        // Here we're checking if we received 4 bytes,
        // and if the client is still in `Pending` state.
        // If their data matches our magic number, they're changed to
        // `Connected` state, other wise they're put in `Disconnected`
        // state.

        if (m_state == Pending) {
            if (bytes_recv == 4) {
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

        m_logger.log("{}\n", m_state);
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

    if (!send) return;

    std::string str = "{\n"
                      "   \"type\": \"disconnect\",\n"
                      "   \"entity\": {\n"
                      "       \"reason\": \"" +
                      reason + "\"\n"
                               "   }\n"
                               "}\n";

    int len = str.size();
    int result = SDLNet_TCP_Send(m_socket, str.c_str(), len);

    // Error sending.
    if (result < len) {
        fmt::print("SDLNet_TCP_Send: {:s}\n", SDLNet_GetError());
        m_state = Disconnected;
    }
}
}
