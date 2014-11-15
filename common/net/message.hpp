#include <functional>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>

#include "common/extlib/json11/json11.hpp"

namespace net {

typedef std::string MessageType;
typedef std::string MessageEntity;
typedef int Socket;

template <class T_handler> class MessageProcessor {

public:
    MessageProcessor(Socket socket) {
        m_socket = socket;
        m_buffer.reserve(8192);
    }

    void addHandler(MessageType type, T_handler handler) {
        m_handlers[type].push_back(handler);
    }

    void proccess() {
        // TODO: Propagation of errors
        auto free_buffer = m_buffer.capacity() - m_buffer.size();
        if (free_buffer == 0) {
            // What do?
            return;
        }
        ssize_t data_or_error = recv(m_socket,
             m_buffer().data() + m_buffer.size(), free_buffer);
        if (data_or_error == 0) {
            return;
        } else if (data_or_error == -1) {
            // Error, need to check errno, may be EAGAIN/EWOULDBLOCK
            return;
        }
        parseBuffer();
    }

    /// @brief Enqueue a message to be sent
    ///
    /// The message will be encoded as a JSON object with two fields: the
    /// `type` and `entity` which will be set to given corresponding
    /// parameters.
    ///
    /// Note that this doesn't necessarily result in the message being sent
    /// immediately. Rather a buffer of pending messages is maintained which
    /// can be sent by a call to `flushSendQueue`.
    ///
    /// The order in which messages are enqueued is guarateed to be the order
    /// they arrive at host they're sent to.
    void send(MessageType type, MessageEntity entity) {
        m_egress.emplace(type, entity);
    }

    /// @brief Encode and send all enqueued messages
    ///
    /// Each JSON message that has been enqueued by send() is encoded into JSON
    /// and is sent over the associated socket with a null temriantor.
    ///
    /// This consumes the send queue entirely.
    void flushSendQueue() {
        while (!m_egress.empty()) {
            json11::Json message = json11::Json::object{
                { "type", std::get<0>(m_egress.front()) },
                { "entity", std::get<1>(m_egress.front()) },
            };
            m_egress.pop();
            std::string encoded_message = message.dump() + " ";
            int sent = 0;
            while (sent < encoded_message.size()) {
                ssize_t data_or_error = ::send(m_socket,
                                             encoded_message.data() + sent,
                                             encoded_message.size() - sent, 0);
                if (data_or_error == -1) {
                    // TODO: Handle/propagate error
                } else {
                    sent = sent + data_or_error;
                }
            }
        }
    }

private:
    Socket m_socket;
    std::string m_buffer;
    std::map<MessageType, std::vector<T_handler>> m_handlers;
    std::queue<std::tuple<MessageType, MessageEntity>> m_ingress;
    std::queue<std::tuple<MessageType, MessageEntity>> m_egress;

    /// @brief Attempt to parse all JSON-encoded messages from the buffer
    ///
    /// This parses all whitespace-delimited JSON objects from the buffer and
    /// calls and adds them to the m_ingress message queue to be dispatched
    /// later.
    ///
    /// Each JSON message should be an object at the top level with a string
    /// `type` field. There should also be a `entity` field which can be of any
    /// type. It is this entity field object that's passed to the message
    /// handler so it's up to the handler to determine validity.
    ///
    /// If a JSON message is not an object, missing the 'type' field or the
    /// type field is the wrong type then the message is ignored. The buffer
    /// will still be consumed as if it were a valid message.
    ///
    /// If the buffer contains incomplete or malformed JSON then no messages
    /// are processed. No messages are added to `m_ingress`. The buffer is not
    /// consumed.
    void parseBuffer() {
        if (m_buffer.empty()) {
            return;
        }
        std::string json_error;
        std::vector<json11::Json> messages =
            json11::Json::parse_multi(m_buffer, json_error);
        // Parsing will fail if the buffer contains a partial message, so the
        // JSON may be well formed but incomplete. This is not ideal. Ideally
        // we should be able to read all the complete messages and only leave
        // the incomplete one in the buffer. This may be an argument in favour
        // of not using `parse_multi`.
        if (json_error.size()) {
            // TODO: Log JSON decode error?
        } else {
            m_buffer.clear();
            for (auto &message : messages) {
                json11::Json type = message["type"];
                // If the 'type' field doesn't exist then is_string()
                // is falsey
                if (type.is_string()) {
                    m_ingress.emplace(type.string_value(), message["entity"]);
                }
            }
        }
    }
};

}  // namesapce net
