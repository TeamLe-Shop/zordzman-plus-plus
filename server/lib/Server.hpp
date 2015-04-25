#pragma once

#include "common/net/message.hpp"

#include "common/logger/Logger.hpp"
#include "json11.hpp"

#include "Client.hpp"
#include "Map.hpp"

#include "Config.hpp"

#include <vector>
#include <fstream>

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

#define RECV_BUFFER_SIZE 1024
#define UDP_PORT 4545
#define TICK_RATE 30

/// The Zordzman server
namespace server {

class Server;
class Client;

typedef net::MessageProcessor<Server *, Client *> Processor;

void handleMapRequest(Processor *, net::MessageEntity, Server *, Client *);

class Server {
public:
    Server(Config config);
    ~Server();
    int exec();

    /// Broadcast a message to all clients
    ///
    /// See Client::send().
    void sendAll(std::string type, json11::Json entity);

    map::Level m_map;
    Config m_config;
    std::vector<Client> m_clients;

private:
    /// Accept all pending connections
    ///
    /// This accept(2)s all pending connections on the listening socket. These
    /// new connections are wrapped in a `Client` and added to the `m_clients`
    /// collection as well has having their socket added to the socket set.
    /// If the max number of clients has been reached the new client will be
    /// disconnected immediately.
    void acceptConnections();

    net::Socket m_tcp_socket;
    struct sockaddr_in m_tcp_address;

    common::Logger m_logger;
#ifdef _WIN32
    WSADATA m_wsa_data;
#endif
};
} // namespace server
