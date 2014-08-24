
#include "Server.hpp"
#include "Client.hpp"

#include <iostream>
#include <string.h>

#include <SDL_net.h>
#include "util.hpp"
#include "format.h"
#include "json11.hpp"

#define PROTOCOL_VERSION 0x00
#define MAGIC_NUMEBER 0xCAC35500 | PROTOCOL_VERSION

Server::Server(IPaddress *address) {
    m_address = address;
    memset(m_recv_buffer, 0, RECV_BUFFER_SIZE);
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
        if (!client_socket) {
            break;
        }
        Client client(client_socket);
        if (client.checkProtocolVersion()) {
            m_clients.push_back(client);
        }
    }
}

int Server::exec() {
    while (true) {
        acceptConnections();
    }
    return 1;
}
