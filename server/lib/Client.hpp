#pragma once

#include <deque>
#include <map>
#include <queue>
#include <string>
#include <vector>

#include "json11.hpp"
#include "common/net/message.hpp"

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>


#include "Server.hpp"

#include "common/logger/Logger.hpp"

#define RECV_BUFFER_SIZE 1024

using namespace net;

namespace server {

class Server;
class Client;

typedef MessageProcessor<Server*, Client*> Processor;

/// Represents a connected client
///
/// When a message handler is called it is passed the client instance from which
/// the message originated. Therefore this used be used for persisting anything
/// about the client that needs to be passed between subsystems.
class Client {

public:
    enum State {
        /// Connection established but protocol version not asserted
        Pending,
        /// Connected and protocol version checked; normal operation
        Connected,
        /// The socket is, or is in the process of disconnecting
        Disconnected,
    };

    /// UDP socket channel, -1 if no channel set yet
    int m_channel;

    /// Construct a new Client instance
    ///
    /// The client's initial state will be set to PENDING.
    Client(struct sockaddr_in addr, int file);

    /// Enqueue a message to be sent to the client
    ///
    /// The message will be encoded as a JSON object with two fields: the
    /// 'type' and 'entity' which will be set to given corresponding
    /// parameters.
    ///
    /// Note that this doesn't send message immediately. Rather a buffer of
    /// pending messages is sent with each call to exec(). Also note that
    /// messages are only actually sent when the client is in the Connected
    /// state, but messages can be enqueued whilst the client is in any state.
    ///
    /// The order in which messages are enqueued is guarateed to be the order
    /// they arrive at the client in.
    void send(std::string type, json11::Json entity);

    // TODO: Rewrite this completely fucking wrong doc string or whatever
    // you call it
    /// Read bytes from the socket into the buffer
    ///
    /// Reads up to RECV_BUFFER_SIZE bytes into the buffer.
    /// Therefore the caller is responsible for calling both
    /// SDLNet_CheckSockets and SDLNet_SocketReady
    /// on the socket set containing the client's socket.
    ///
    /// Returns all the messages that were received by the client.
    std::vector<json11::Json> exec();

    /// Disconnect for `reason`
    ///
    /// This will set client's state to Disconnected and optionally send the
    /// reason to the client. Sending the disconnect reason to the server
    /// will result in the send queue being flushed. It's generally ill-advised
    /// to do this before the client at least enters the Connected state.
    ///
    /// Regardless of whether or not the send queue is flushed a message will
    /// be added to the queue with the type 'disconnect'. The message entity
    /// will be set to the reason string.
    ///
    /// @param reason Reason for disconnection
    /// @param flush Whether or not to flush the send buffer? If false then the
    ///     disconnect reason will never reach the client.
    void disconnect(std::string reason, bool flush);
    void disconnect(std::string reason);
    void disconnect();

    State getState() const;

    // Forbid copying
    Client(const Client &) = delete;
    Client &operator=(const Client &) = delete;

    // Move operations
    Client(Client &&);
    Client &operator=(Client &&);

    // Destructor
    ~Client();

    Socket m_tcp_socket;
    Socket m_udp_socket;

    Processor m_msg_proc;

    State m_state;

    /// Assert the client is using the correct protocol version
    ///
    /// If the client state is Pending this checks if the buffer contains the
    /// magic number. If the correct magic number is at the front of the buffer
    /// then the client state is set to connected. If the front of the buffer
    /// doesn't match the magic number the client is disconnected.
    ///
    /// If the client is any state other than Pending or the buffer does not
    /// at least contain the minimum number of bytes for the magic number,
    /// this method has no effect.
    ///
    /// The magic number is consumed from the buffer.
    void checkProtocolVersion(std::string buffer);

private:
    std::string magic_buffer;
    common::Logger m_logger;

};
} // namespace server
