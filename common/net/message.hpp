#pragma once

#include <functional>
#include <map>
#include <queue>
#include <string>
#include <tuple>
#include <vector>
#include <cstring>
#include <cerrno>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#endif

#include "format.h"
#include "common/extlib/json11/json11.hpp"

#include "common/entity/entity.hpp"

/// Networking utilities common to both the server and client
namespace net {

typedef std::string MessageType;
typedef json11::Json MessageEntity;
typedef int Socket;

/// Handle sending and receiving JSON-encoded messages over a socket.
///
/// This class operators on a TCP socket to communicate whitespace-separated,
/// JSON-encoded messages. Each 'message' is a JSON object with two fields.
/// The `type` field is a string that identifies the type of the message and
/// is used to map messages to callbacks.
///
/// Secondly, the `entity` field can be any JSON type. A string, a number or
/// even a complex data structure comprised of multiple nested objects and
/// arrays. Anything is valid in this field. However the structure of the
/// `entity` field is implied by the value of the `type` field. As such, all
/// messages of a given type should conform to a defined structure.
///
/// Note that this class provides no mechanism for validating message entities
/// so message handlers must implement that themselves.
///
/// @code{.json}
/// {"type": "example", "entity": ...}
/// @endcode
///
/// As mentioned above, this class introduces a concept of callbacks/handlers.
/// These are void-returning callable objects that are called in response to
/// messages of certain types being received. Handlers must be registered on
/// `MessageProcessor` instances via `MessageProcessor::addHandler`.
///
/// This class is generic and can deal with handlers with any signature by
/// specifying the handler signature in the type declaration. For example:
///
/// @code
/// net::MessageProcessor<int, std::string>
/// @endcode
///
/// Handlers for the above message processor will accept an `int` and
/// `std::string` argument. However, as well as this, there are two implicit
/// leading arguments. The first is a pointer to the `MessageProcessor` that
/// the handler is registered on and the second is the message entity field
/// as a `json11::Json` (for which the `MessageEntity` typedef is provided
/// for convenience). Therefore, a handler for the above message processor
/// would look like:
///
/// @code
/// typedef net::MessageProcessor<int, std::String> Processor;
///
/// void handler(Processor *processor,
///              net::MessageEntity entity, int first, std::string second) {
///       processor->send("echo", entity);
///  }
///
/// Processor processor(...);
/// processor.addHandler("example", handler);
/// processor.dispatch(5, "foo");
/// @endcode
///
/// The handlers can use the processor pointer in order to `send` responses.
///
/// There is a slight variation on handlers which are referred to as '*muted
/// handlers*'. Muted handlers are the same as regular handlers except they
/// don't accept the initial pointer to the processor instance, therefore they
/// are unable to `send` responses. Continuing from the above example:
///
/// @code
/// void muted_handler(net::MessageEntity entity,
///                    int first, std::string second) {
///     // Mmph mphna mprh.
/// }
///
/// processor.addHandler("example", muted_handler);
/// processor.dispatch(5, "foo");
/// @endcode
template <class ... Args> class MessageProcessor {

using Handler = std::function<void(
    MessageProcessor<Args ...> *,
    MessageEntity,
    Args ...
)>;

using MutedHandler = std::function<void(
    MessageEntity,
    Args ...
)>;

public:
    MessageProcessor() { m_buffer.reserve(8192); }
    /// @param socket A connected socket descriptor
    MessageProcessor(Socket socket) {
        m_socket = socket;
        m_buffer.reserve(8192);
    }

    /// Set socket descriptor for this processor
    void setSocket(Socket socket) {
        m_socket = socket;
    }

    /// Register a callback for a given message type
    ///
    /// The callback -- or rather, the *handler* -- should accept two implicit
    /// initial arguments. The first is a pointer to the calling message
    /// proccesor instance, and the second is the MessageEntity for the message
    /// the handler is called for.
    ///
    /// When a message is received that has a type that matches the one the
    /// handler is registered against, then the handler is called being passed
    /// the message's entity as the second parameter.
    ///
    /// Multiple handlers can be registered for a single type. Each handler is
    /// called once for each message received.
    void addHandler(MessageType type, Handler handler) {
        m_handlers[type].push_back(handler);
    }

    /// Register a muted callback for a given message type
    ///
    /// Muted handlers are the same as other handlers except they don't
    /// accept a pointer to the message processor as the first argument.
    /// Because of this they are unable to send messages back, effectively
    /// making them read-only or 'muted'.
    ///
    /// This mostly exists to save keystrokes.
    void addHandler(MessageType type, MutedHandler handler) {
        addHandler(type, [handler](MessageProcessor<Args ...> */*processor*/,
                MessageEntity entity, Args ... args){
            return handler(entity, args ...);
        });
    }

    /// Call all handlers for recieved messages
    ///
    /// This will call all the handlers for each message that has been received
    /// by calls to `process`. The given `args` are passed through to the
    /// handler calls.
    void dispatch(Args ... args) {
        while (!m_ingress.empty()) {
            for (auto &handler : m_handlers[std::get<0>(m_ingress.front())]) {
                handler(this, std::get<1>(m_ingress.front()), args ...);
            }
            m_ingress.pop();
        }
    }

    /// Receive and parse messages
    ///
    /// This will attempt to receive JSON-encoded messages from the associated
    /// socket. Note that this method doesn't call the message handlers
    /// immediately. Instead they are enqueued for deferred dispatching via
    /// `dispatch`.
    ///
    /// The order the messages are received is the same order they'll be
    /// dispatched.
    bool process() {
        // TODO: Propagation of errors
        auto free_buffer = m_buffer.capacity() - m_buffer.size();
        if (free_buffer == 0) {
            // What do?
            fmt::print("No free buffer.\n");
        }

        int len = strlen(&m_buffer[0]);
        m_buffer.resize(free_buffer);
        ssize_t data_or_error = recv(m_socket, &m_buffer[len],
                                     free_buffer, 0);
        len = strlen(&m_buffer[0]);
        if (data_or_error == 0) {
            m_buffer.resize(0);
            return false;
        } else if (data_or_error == -1) {
            // Error, need to check errno, may be EAGAIN/EWOULDBLOCK

            if (errno != EAGAIN) {
                fmt::print("(MessageProcessor) Error receiving: {}\n",
                            strerror(errno));
                return false;
            }
            m_buffer.resize(len);
            return true;
        }
        m_buffer.resize(len);
        parseBuffer();
        return true;
    }

    /// Enqueue a message to be sent
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

    /// Enqueue a message that represents an entity's change of state
    ///
    /// This will enqueue a new message of type `entity.state`. These messages
    /// are used to convey a change in an entity component's field.
    ///
    /// The message entities are objects with the following fields:
    ///
    /// - `entity` -- the numeric ID of the entity.
    /// - `component` -- a string identifying the component on the entity.
    /// - `field` -- a string identifying the name of the field on the
    ///              component.
    /// - `value` -- the new value of the field. The type of this field is
    ///              dependent on the component and field.
    ///
    /// N.B. the order of state change messages matters. This method guarantees
    /// that state change messages arrive at the endpoint in the order they
    /// were enqueued. The most recent state change should always be sent last.
    ///
    /// @param change A four-tuple containing the entity ID as an int, the
    ///               component name as a std::string, the field name as a
    ///               std::string and the field value as a json11::Json.
    void sendStateChange(entity::StateChange change) {
        json11::Json entity = json11::Json::object{
            { "id", (int) std::get<0>(change) },
            { "component", std::get<1>(change) },
            { "field", std::get<2>(change) },
            { "value", std::get<3>(change) },
        };
        send("entity.state", entity);
    }

    /// Encode and send all enqueued messages
    ///
    /// Each JSON message that has been enqueued by send() is encoded into JSON
    /// and is sent over the associated socket with a null terminator.
    ///
    /// This consumes the send queue entirely.
    bool flushSendQueue() {
        while (!m_egress.empty()) {

            json11::Json message = json11::Json::object{
                { "type", std::get<0>(m_egress.front()) },
                { "entity", std::get<1>(m_egress.front()) },
            };
            m_egress.pop();
            std::string encoded_message = message.dump() + " ";
            ssize_t sent = 0;
            while (sent < encoded_message.size()) {
                ssize_t data_or_error = ::send(m_socket,
                                             encoded_message.data() + sent,
                                             encoded_message.size() - sent, 0);
                if (data_or_error == -1) {
                    fmt::print("(MessageProcessor) Error sending: {}\n",
                       strerror(errno));
                    return false;
                } else {
                    sent = sent + data_or_error;
                }
            }
        }
        return true;
    }

private:
    Socket m_socket;
    std::string m_buffer;
    std::map<MessageType, std::vector<Handler>> m_handlers;
    std::queue<std::tuple<MessageType, MessageEntity>> m_ingress;
    std::queue<std::tuple<MessageType, MessageEntity>> m_egress;

    /// Attempt to parse all JSON-encoded messages from the buffer
    ///
    /// This parses all whitespace-delimited JSON objects from the buffer and
    /// and adds them to the m_ingress message queue to be dispatched
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
            json11::Json::parse_multi(m_buffer.c_str(), json_error);
        // Parsing will fail if the buffer contains a partial message, so the
        // JSON may be well formed but incomplete. This is not ideal. Ideally
        // we should be able to read all the complete messages and only leave
        // the incomplete one in the buffer. This may be an argument in favour
        // of not using `parse_multi`.
        if (json_error.size()) {
            printf("(MessageProcessor) JSON decode error: %s\n",
                   json_error.c_str());
        } else {
            m_buffer.clear();
            for (json11::Json message : messages) {
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

}  // namespace net
