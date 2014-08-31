
#include "Server.hpp"
#include "Client.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string.h>

#include <SDL_net.h>
#include "util.hpp"
#include "format.h"
#include "json11.hpp"

#define PROTOCOL_VERSION 0x00
#define MAGIC_NUMBER 0xCAC35500 | PROTOCOL_VERSION

namespace server {
Server::Server(IPaddress *address, unsigned int max_clients) {
    m_address = address;
    m_max_clients = max_clients;
    m_socket_set = SDLNet_AllocSocketSet(m_max_clients * 1);
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);

    fmt::print("[INFO] Compiled with SDL_net version: {:d}.{:d}.{:d}\n",
               compile_version.major, compile_version.minor,
               compile_version.patch);
    fmt::print("[INFO] Running with SDL_net version: {:d}.{:d}.{:d}\n\n",
               link_version->major, link_version->minor, link_version->patch);
    if (SDL_Init(0) == -1) {
        fprintf(stderr, "[ERROR] SDL_Init: %s\n", SDL_GetError());
        fprintf(stderr,
                "[ERROR] Failed to initialize SDL. Quitting zordz-server...\n");
        exit(1);
    }
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "[ERROR] SDLNet_Init: %s\n", SDLNet_GetError());
        fprintf(stderr,
                "[ERROR] Failed to initialize SDL. Quitting zordz-server...\n");
        exit(1);
    }

    if (!(m_socket = SDLNet_TCP_Open(address))) {
        log("Failed to bind to interface", IPaddress_AsString(address));
    }
    log("Bound to interface", IPaddress_AsString(m_address));
}

Server::~Server() { printf("[INFO] Server shut down.\n\n"); }

void Server::acceptConnections() {
    while (true) {
        // Returns immediately with NULL if no pending connections
        TCPsocket client_socket = SDLNet_TCP_Accept(m_socket);

        if (!client_socket) break;

        if (m_clients.size() >= m_max_clients) {
            // Perhaps issue some kind of "server full" warning. But how would
            // this be done as the client would be in the PENDING state
            // intially.
            SDLNet_TCP_Close(client_socket);
        } else {
            Client client(client_socket);
            m_clients.push_back(client);
            SDLNet_TCP_AddSocket(m_socket_set, client_socket);
        }
    }
}

int Server::exec() {
    while (true) {
        acceptConnections();
        SDLNet_CheckSockets(m_socket_set, 0);
        for (auto client : m_clients) {
            if (client.m_state == PENDING
                || client.m_state == CONNECTED) {
                client.recv();
            }
        }
        // lol is copy neccesary? can i into modify in-place?
        std::vector<Client> filtered_clients;
        std::copy_if(
            m_clients.begin(),
            m_clients.end(),
            std::back_inserter(filtered_clients),
            [] (Client client) {
                return client.m_state == DISCONNECTED;
            }
        );
        m_clients = filtered_clients;
    }
    return 1;
}
}
