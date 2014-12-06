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
#include <unistd.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

namespace cont = common::util::container;

namespace server {

using namespace std::placeholders;
using namespace json11;

Server::Server(int port, unsigned int max_clients,
               std::string map_name)
    : m_logger(stderr, [] { return "SERVER: "; }) {
    m_max_clients = max_clients;

    m_map.loadLevel(map_name);
    // Log this in the map loader maybe?
    m_logger.log("Map hash: {}", m_map.md5.getHash());

#   ifndef IPV4_ONLY
#   define host_str NULL
#   define DIGIT_STRING_LENGTH(num) (1                                   /* 1 for sign */ \
                                   + sizeof (num) * CHAR_BIT / 3         /* ... for digits */ \
                                   + (sizeof (num) * CHAR_BIT % 3 > 0)   /* ... for remaining digit */ \
                                   + 1)                                  /* 1 for NUL terminator */
    char port_str[DIGIT_STRING_LENGTH(port)];
    sprintf(port_str, "%d", port);

    if (getaddrinfo(host_str, port_str, &(struct addrinfo){ .ai_family = PF_UNSPEC, .ai_socktype = SOCK_STREAM, .ai_flags = AI_PASSIVE }, &m_tcp_address) != 0) {
        m_logger.log("[ERR]  Failed to resolve local stream interface: {}",
                     strerror(errno));
    }

    for (struct addrinfo *a = m_tcp_address; a != NULL; a = a->ai_next) {
        Socket s = socket(a->ai_family, a->ai_socktype, a->ai_protocol);
        if (s < 0) {
            m_logger.log("[WARNING]  Failed to create socket: {}", strerror(errno));
            continue;
        }

        if (bind(s, a->ai_addr, a->ai_addrlen) < 0) {
            m_logger.log("[WARNING]  Failed to bind socket: {}", strerror(errno));
            goto cleanup;
	    }

        if (listen(s, SOMAXCONN) == -1) {
            m_logger.log("[WARNING]  Failed to listen socket: {}", strerror(errno));
            goto cleanup;
        }

        if ((fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK) == -1) {
            m_logger.log("[WARNING]  Failed to fcntl socket: {}", strerror(errno));
            goto cleanup;
        }


        char host[1024];
        char service[64];
        if (getnameinfo(a->ai_addr, a->ai_addrlen, host, sizeof host, service, sizeof service, 0) == 0) {
            strcpy(host, "UNKNOWN");
            strcpy(service, "UNKNOWN");
        }

        m_tcp_socket.push_back(s);
        m_logger.log("[INFO] Bound to interface {}, service {}", std::string(host), std::string(service));
        continue;
cleanup:close(s);
    }

    if (m_tcp_socket.size() == 0) {
        m_logger.log("[ERR]  Failed to bind to a stream interface");
        exit(1);
    }
#   else
    if ((m_tcp_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        m_logger.log("[ERR]  Failed to create socket: {}", strerror(errno));
        exit(1);
    }

    fcntl(m_tcp_socket, F_SETFL, O_NONBLOCK);

    memset(&m_tcp_address, 0, sizeof m_tcp_address);

    m_tcp_address.sin_family = AF_INET;
    m_tcp_address.sin_port   = htons(port);

    if (INADDR_ANY) {
        m_tcp_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    if (bind(m_tcp_socket, (const struct sockaddr *)&m_tcp_address,
        sizeof m_tcp_address) < 0) {
        m_logger.log("[ERR]  Failed to bind TCP interface: {}",
                     strerror(errno));
    }

    listen(m_tcp_socket, SOMAXCONN);

//  if (!(m_udp_socket = socket(AF_INET, SOCK_DGRAM, 0) )) {
//      m_logger.log("[ERR]  Failed to bind UDP interface: {}",
//                   strerror(errno));
//      exit(1);
//  }
    m_logger.log("[INFO] Bound to interface {}",
                 common::util::net::ipaddr(m_tcp_address));
#   endif

    addHandler("map.request",
               std::bind(&server::Server::handleMapRequest, this, _1, _2, _3));
    //addHandler("net.udp",
    //          std::bind(&server::Server::handleNetUDP, this, _1, _2, _3));
}

Server::~Server() { m_logger.log("[INFO] Server shut down.\n\n"); }

void Server::sendAll(std::string type, Json entity) {
    for (auto &client : m_clients) {
        client.send(type, entity);
    }
}

void Server::addHandler(std::string type,
                        std::function<void(Server *server, Client *client,
                                           json11::Json entity)> handler) {
    m_handlers[type].push_back(handler);
}

void Server::handleMapRequest(Server */*server*/, Client *client,
                              json11::Json /*entity*/) {
    client->send("map.contents", m_map.asBase64());
}

void Server::handleNetUDP(Server */*server*/,
                          Client */*client*/, json11::Json /*entity*/) {
}

void Server::acceptConnections() {
#   ifndef IPV4ONLY
    iterator s = m_tcp_socket.begin();
    while (s != m_tcp_socket.end()) {
        struct sockaddr_storage a;
        socklen_t n = sizeof a;
        Socket c = accept(*s, &a, &n);
        if (c < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                m_logger.log("[WARNING]  Failed to accept client connection: {}", strerror(errno));
            }
            goto next;
        }

        if (fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK) == -1) {
            m_logger.log("[WARNING]  Failed to fcntl socket: {}", strerror(errno));
            goto next;
        }
        
        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially?
            close(client_socket);
            continue;
        }


        m_clients.emplace_back(a, c); /* XXX: Looks like Client might need a touch of work to accept sockaddr_storage rather than sockaddr_in */
        m_clients.back().send("map.offer", m_map.md5.getHash());
        m_clients.back().send("net.udp", UDP_PORT);
        continue;
next:   s++;
    }
#   else
    socklen_t b = sizeof(m_tcp_socket);
    while (true) {
        // Returns immediately with NULL if no pending connections
        int client_socket = accept(m_tcp_socket,
                                   (struct sockaddr *)&m_tcp_address, &b);

        if (client_socket < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                m_logger.log("Failed to accept client connection: {}",
                         strerror(errno));
                break;
            } else {
                break;
            }
        }

        struct sockaddr peer_address;
        socklen_t addrlen = sizeof(peer_address);
        int error = getpeername(client_socket, &peer_address,
                                &addrlen);
        if (error == -1) {
            throw std::runtime_error("Error getting peer name.");
        }
        struct sockaddr_in *addr_in = (struct sockaddr_in *)&peer_address;

        fcntl(client_socket, F_SETFL, O_NONBLOCK);

        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially?
            close(client_socket);
        } else {
            m_clients.emplace_back(*addr_in, client_socket);
            m_clients.back().send("map.offer", m_map.md5.getHash());
            m_clients.back().send("net.udp", UDP_PORT);
        }
    }
#   endif
}

int Server::exec() {
    while (true) {
        acceptConnections();
        for (auto &client : m_clients) {
            for (auto &message : client.exec()) {
                // We can't use message.has_shape() here because we don't want
                // to make assumptions about the type of the message entity
                if (message.is_object()) {
                    Json type = message["type"];
                    // If the 'type' field doesn't exist then is_string()
                    // is falsey
                    if (type.is_string()) {
                        for (auto &handler : m_handlers[type.string_value()]) {
                            handler(this, &client, message["entity"]);
                        }
                    }
                }
            }
        }
        // Remove disconnected clients
        for (size_t i = 0; i < m_clients.size(); ++i) {
            Client &client = m_clients[i];

            if (client.getState() == Client::Disconnected) {
                close(client.m_tcp_socket);
                m_clients.erase(m_clients.begin() + i);
            }
        }
    }

    return 1;
}
} // namespace server
