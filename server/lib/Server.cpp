#include "Server.hpp"
#include "Client.hpp"
#include "common/util/container.hpp"
#include "common/extlib/hash-library/md5.h"
#include "common/util/stream.hpp"
#include "common/util/net.hpp"
#include "Map.hpp"

#include <format.h>
#include <json11.hpp>

#include <cstdio>
#include <cerrno>
#include <string.h>
#include <fcntl.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#include <windows.h>
#endif

#define TICK_RATE 30

namespace cont = common::util::container;

namespace server {

using namespace std::placeholders;
using namespace json11;

void handleMapRequest(Processor *, MessageEntity /*entity*/, Server *server,
                      Client *client) {
    if (!server->m_allow_downloads) {
        client->disconnect("Downloads not enabled.", true);
        return;
    }
    client->m_msg_proc.send("map.contents", server->m_map.asBase64());
}

Server::Server(int port, unsigned int max_clients, std::string map_name,
               bool allow_downloads)
    : m_logger(stderr, [] { return "SERVER: "; }) {
    m_max_clients = max_clients;
    m_allow_downloads = allow_downloads;

    m_map.loadLevel(map_name);
    // Log this in the map loader maybe?
    m_logger.log("[INFO] Map hash: {}", m_map.md5.getHash());

    if (!m_allow_downloads) {
        m_logger.log("[INFO] Downloads not enabled.");
    }
#ifdef _WIN32
    WSAStartup(MAKEWORD(2, 2), &m_wsa_data);
    if ((m_tcp_socket = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        int err = WSAGetLastError();
        m_logger.log("[ERR]  Failed to create socket: (wsagetlasterror: {})",
                     err);
        exit(1);
    }
#else
    if ((m_tcp_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        m_logger.log("[ERR]  Failed to create socket: {}", strerror(errno));
        exit(1);
    }
#endif

    int optval = 1;
    setsockopt(m_tcp_socket, SOL_SOCKET, SO_REUSEADDR,
               reinterpret_cast<const char *>(&optval), sizeof(optval));

#ifdef _WIN32
    ioctlsocket(m_tcp_socket, FIONBIO, nullptr);
#else
    fcntl(m_tcp_socket, F_SETFL, O_NONBLOCK);
#endif

    memset(&m_tcp_address, 0, sizeof m_tcp_address);

    m_tcp_address.sin_family = AF_INET;
    m_tcp_address.sin_port = htons(port);

    if (INADDR_ANY) {
        m_tcp_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(m_tcp_socket, (const struct sockaddr *)&m_tcp_address,
             sizeof m_tcp_address) < 0) {
        m_logger.log("[ERR]  Failed to bind TCP interface: {}",
                     strerror(errno));
        exit(1);
    }

    listen(m_tcp_socket, SOMAXCONN);

    //  if (!(m_udp_socket = socket(AF_INET, SOCK_DGRAM, 0) )) {
    //      m_logger.log("[ERR]  Failed to bind UDP interface: {}",
    //                   strerror(errno));
    //      exit(1);
    //  }
    m_logger.log("[INFO] Bound to interface {}",
                 common::util::net::ipaddr(m_tcp_address));
}

Server::~Server() {
#ifdef _WIN32
    WSACleanup();
#endif
    m_logger.log("[INFO] Server shut down.\n\n");
}

void Server::sendAll(std::string type, Json entity) {
    for (auto &client : m_clients) {
        client.m_msg_proc.send(type, entity);
    }
}

void Server::acceptConnections() {
    socklen_t b = sizeof(m_tcp_socket);
    while (true) {
        // Returns immediately with nullptr if no pending connections
        Socket client_socket =
            accept(m_tcp_socket, (struct sockaddr *)&m_tcp_address, &b);
#ifdef _WIN32
        if (client_socket == INVALID_SOCKET) {
            int err = WSAGetLastError();
            m_logger.log("[ERR]  Failed to accept client connection: "
                         "(wsagetlasterror: {})",
                         err);
            break;
        } else {
            break;
        }
#else
        if (client_socket < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                m_logger.log("[ERR]  Failed to accept client connection: {}",
                             strerror(errno));
                break;
            } else {
                break;
            }
        }
#endif

        struct sockaddr peer_address;
        socklen_t addrlen = sizeof(peer_address);
        int error = getpeername(client_socket, &peer_address, &addrlen);
        if (error == -1) {
            throw std::runtime_error("Error getting peer name.");
        }
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&peer_address;
#ifdef _WIN32
        ioctlsocket(client_socket, FIONBIO, nullptr);
#else
        fcntl(client_socket, F_SETFL, O_NONBLOCK);
#endif
        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially?
            close(client_socket);
        } else {
            m_clients.emplace_back(*addr_in, client_socket);
            m_clients.back().m_msg_proc.setSocket(client_socket);
            m_clients.back().m_msg_proc.addHandler("map.request",
                                                   handleMapRequest);
            m_clients.back().m_msg_proc.send(
                "map.offer", Json::object{ { "name", m_map.name },
                                           { "hash", m_map.md5.getHash() } });
            sendAll("server.message",
                    Json::object{ { "message",
                                    fmt::format("{} has connected.",
                                                common::util::net::ipaddr(
                                                    *addr_in, false)) } });
        }
    }
}

int Server::exec() {
    while (true) {
#ifdef _WIN32
        Sleep((1000 / TICK_RATE));
#else
        usleep((1000 / TICK_RATE) * 1000);
#endif
        acceptConnections();
        for (auto &client : m_clients) {
            if (client.getState() == Client::Pending) {
                client.checkProtocolVersion();
                if (client.getState() == Client::Connected) {
                    client.decideClientName(m_clients);
                    m_map.addPlayer("Player");
                }
                continue;
            }
            if (!client.m_msg_proc.flushSendQueue()) {
                client.disconnect("Failed to send to client", false);
                continue;
            }
            client.exec(this);
        }
        // Remove disconnected clients
        for (size_t i = 0; i < m_clients.size(); ++i) {
            Client &client = m_clients[i];

            if (client.getState() == Client::Disconnected) {
                sendAll("server.message",
                        Json::object{ { "message",
                                        fmt::format("{} left the game.",
                                                    common::util::net::ipaddr(
                                                        client.m_addr)) } });
                close(client.m_tcp_socket);
                m_clients.erase(m_clients.begin() + i);
            }
        }
        m_map.cycle();
    }

    return 1;
}

} // namespace server
