#include "Server.hpp"
#include "Client.hpp"
#include "util.hpp"
#include "common/util/container.hpp"
#include "common/extlib/hash-library/md5.h"
#include "common/util/stream.hpp"
#include "Map.hpp"

#include <format.h>
#include <json11.hpp>
#include <SDL_net.h>

namespace cont = common::util::container;

namespace server {

using namespace std::placeholders;
using namespace json11;

Server::Server(IPaddress address, unsigned int max_clients,
               std::string map_name)
    : m_logger(stderr, [] { return "SERVER: "; }) {
    m_address = address;
    m_max_clients = max_clients;
    m_socket_set = SDLNet_AllocSocketSet(m_max_clients);

    initSDL();
    m_map.loadLevel(map_name);
    // Log this in the map loader maybe?
    m_logger.log("Map hash: {}", m_map.md5.getHash());

    if (!(m_socket = SDLNet_TCP_Open(&address))) {
        m_logger.log("[ERR]  Failed to bind TCP to interface {}", address);
        exit(1);
    }
    if (!(m_udp_socket = SDLNet_UDP_Open(UDP_PORT))) {
        m_logger.log("[ERR]  Failed to bind UDP interface");
        exit(1);
    }
    m_logger.log("[INFO] Bound to interface {}", m_address);
    addHandler("map.request",
               std::bind(&server::Server::handleMapRequest, this, _1, _2, _3));
    addHandler("net.udp",
               std::bind(&server::Server::handleNetUDP, this, _1, _2, _3));
}

Server::~Server() { m_logger.log("[INFO] Server shut down.\n\n"); }

void Server::initSDL() {
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);

    m_logger.log("[INFO] Compiled with SDL_net version: {:d}.{:d}.{:d}",
                 compile_version.major, compile_version.minor,
                 compile_version.patch);
    m_logger.log("[INFO] Running with SDL_net version: {:d}.{:d}.{:d}\n",
                 link_version->major, link_version->minor, link_version->patch);
    if (SDL_Init(0) == -1) {
        m_logger.log("[ERR]  SDL_Init: {}", SDL_GetError());
        m_logger.log("[ERR]  Failed to initialize SDL. Quitting "
                     "zordzman-server...\n");
        exit(1);
    }
    if (SDLNet_Init() == -1) {
        m_logger.log("[ERR]  SDLNet_Init: {}\n", SDLNet_GetError());
        m_logger.log("[ERR]  Failed to initialize SDLNet. Quitting"
                     " zordzman-server...\n");
        exit(1);
    }
}

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

void Server::handleMapRequest(Server *server, Client *client,
                              json11::Json entity) {
    client->send("map.contents", m_map.asBase64());
}

void Server::handleNetUDP(Server *server,
                          Client *client, json11::Json entity) {
    if (entity.is_number()) {
        IPaddress *remote = SDLNet_TCP_GetPeerAddress(client->getSocket());
        if (!remote) {
            client->disconnect(SDLNet_GetError(), true);
        } else {
            remote->port = entity.int_value();
            int channel = SDLNet_UDP_Bind(m_udp_socket, -1, remote);
            if (channel == -1) {
                client->disconnect(
                    fmt::format("Unable to allocate channel: {}",
                                SDLNet_GetError()), true);
            } else {
                client->m_channel = channel;
            }
        }
    }
}

void Server::acceptConnections() {
    while (true) {
        // Returns immediately with NULL if no pending connections
        TCPsocket client_socket = SDLNet_TCP_Accept(m_socket);

        if (!client_socket) {
            break;
        }

        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially?
            SDLNet_TCP_Close(client_socket);
        } else {
            m_clients.emplace_back(client_socket);
            m_clients.back().send("map.offer", m_map.md5.getHash());
            m_clients.back().send("net.udp", UDP_PORT);
            SDLNet_TCP_AddSocket(m_socket_set, client_socket);
        }
    }
}

int Server::exec() {
    while (true) {
        acceptConnections();
        SDLNet_CheckSockets(m_socket_set, 1);
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
                SDLNet_TCP_DelSocket(m_socket_set, client.getSocket());
                if (client.m_channel != -1) {
                    // We might get disconnected before we even get chance
                    // to  assign a channel
                    SDLNet_UDP_Unbind(m_udp_socket, client.m_channel);
                    client.m_channel = -1;
                }
                m_clients.erase(m_clients.begin() + i);
            }
        }
    }

    return 1;
}
} // namespace server
