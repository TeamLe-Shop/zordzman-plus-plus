#include "Client.hpp"
#include "format.h"
#include "util.hpp"

// Last octet can be the protocol version if we ever decide to care
#define MAGIC_NUMBER "\xCA\xC3\x55\x01"

namespace server {

using namespace json11;

void hello_handler(Client *client, Json entity) {
    if (entity.is_string()) {
        fmt::print("Entity: {}\n", entity.string_value());
    }
}

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
    m_logger.log("Client connected (state = Pending)");
    addHandler("hello", hello_handler);
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
            m_buffer.push_back(buffer[i]);
        }
    }
    checkProtocolVersion();
    if (m_state == Connected) {
        processMessages();
    }
}

void Client::addHandler(std::string type,
        void (*handler)(Client *, json11::Json)) {
    m_handlers[type].push_back(handler);
    return;
}

void Client::processMessages() {
    if (m_buffer.empty()) {
        return;
    }
    std::string json_error;
    // Well this seems stupidly inefficient. Why can't m_buffer be a
    // std::string?
    std::string raw(m_buffer.begin(), m_buffer.end());
    std::vector<json11::Json> messages =
        json11::Json::parse_multi(raw, json_error);
    // Parsing will fail if the buffer contains a partial message, so the JSON
    // may be well formed but incomplete. This is not ideal. Ideally we should
    // be able to read all the complete messages and only leave the incomplete
    // one in the buffer.
    if (json_error.size()) {
        m_logger.log(fmt::format("JSON decode failed: {}", json_error));
    } else {
        for (auto &message : messages) {
            // We can't use message.has_shape() here because we don't want to
            // make assumptions about the type of the message entity
            if (message.is_object()) {
                json11::Json type = message["type"];
                // If the 'type' field doesn't exist then is_string() is falsey
                if (type.is_string()) {
                    for (auto &handler : m_handlers[type.string_value()]) {
                        handler(this, message["entity"]);
                    }
                }
            }
        }
        // Consume the buffer
        m_buffer.erase(m_buffer.begin(), m_buffer.end());
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
