#include "Client.hpp"
#include "format.h"
#include "util.hpp"

// Last octet can be the protocol version if we ever decide to care
#define MAGIC_NUMBER "\xCA\xC3\x55\x01"

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
    if (m_buffer.size() < strlen(MAGIC_NUMBER)) {
        return;
    } else {
        char magic[] = MAGIC_NUMBER;
        for (std::size_t i = 0; i < strlen(MAGIC_NUMBER); i++) {
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

std::vector<Json> Client::exec() {
    if (m_state == Disconnected) {
        return std::vector<Json>();
    }
    char buffer[RECV_BUFFER_SIZE];
    memset(buffer, 0, RECV_BUFFER_SIZE);
    auto orig_buffer_size = m_buffer.size();
    if (SDLNet_SocketReady(m_socket)) {
        int bytes_recv = SDLNet_TCP_Recv(m_socket, buffer,
                                         RECV_BUFFER_SIZE - m_buffer.size());
        m_logger.log(fmt::format("Bytes received: {}", bytes_recv));
        if (bytes_recv <= 0) {
            // Socket is likely closed so there's no reason to send the
            // disconnect message
            disconnect(
                fmt::format("Left server (recv: {})", bytes_recv), false);
        }
        for (int i = 0; i < bytes_recv; i++) {
            m_buffer.push_back(buffer[i]);
        }
    }
    checkProtocolVersion();
    if (m_state == Connected) {
        flushSendQueue();
        if (m_buffer.size() != orig_buffer_size) {
            // If the buffer hasn't changed size then its not in a parsable
            // state or its empty
            return processMessages();
        }
    }
    return std::vector<Json>();
}

void Client::send(std::string type, Json entity) {
    Json message = Json::object{
        { "type", type }, { "entity", entity },
    };
    m_send_queue.push(message);
}

void Client::flushSendQueue() {
    while (!m_send_queue.empty()) {
        json11::Json message = m_send_queue.front();
        m_send_queue.pop();
        std::string encoded_message = message.dump() + " ";
        // Using cppformat or the logger with the encoded_message causes
        // wierdness I don't understand
        printf("Send: %s\n", encoded_message.c_str());
        if (SDLNet_TCP_Send(
                m_socket,
                encoded_message.data(),
                encoded_message.length()) < (int)encoded_message.length()) {
            // We just failed a flush, don't try to flush again whilst
            // disconnecting
            disconnect(
                fmt::format("Failed to send: {}", SDLNet_GetError()), false);
        }
    }
}

std::vector<Json> Client::processMessages() {
    if (m_buffer.empty()) {
        return std::vector<Json>();
    }
    std::string json_error;
    // Well this seems stupidly inefficient. Why can't m_buffer be a
    // std::string?
    std::string raw(m_buffer.begin(), m_buffer.end());
    std::vector<Json> messages = Json::parse_multi(raw, json_error);
    // Parsing will fail if the buffer contains a partial message, so the JSON
    // may be well formed but incomplete. This is not ideal. Ideally we should
    // be able to read all the complete messages and only leave the incomplete
    // one in the buffer.
    if (json_error.size()) {
        m_logger.log(fmt::format("JSON decode failed: {}", json_error));
    } else {
        // Consume the buffer
        m_buffer.erase(m_buffer.begin(), m_buffer.end());
    }
    return messages;
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

void Client::disconnect(std::string reason, bool flush) {
    send("disconnect", reason);
    if (flush) {
        flushSendQueue();
    }
    m_state = Disconnected;
    m_logger.log("Client disconnected (state = Disconnected): {} ", reason);
}

void Client::disconnect(std::string reason) {
    disconnect(reason, true);
}

void Client::disconnect() {
    disconnect("You have been disconnected", true);
}
} // namespace server
