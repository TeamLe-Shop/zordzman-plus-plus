#include "Server.hpp"
#include "Client.hpp"

#include <SDL_net.h>
#include <iostream>
#include <string.h>
#include "format.h"
#include "json11.hpp"

#define PORT_NUMBER 4544
#define PROTOCOL_VERSION 0

using fmt::print;

namespace {
void hoststr(char *str, IPaddress *remoteIP) {

    Uint32 ip = SDLNet_Read32(&remoteIP->host);
    Sint16 port = SDLNet_Read16(&remoteIP->port);
    memset(str, 0, strlen(str));
    sprintf(str, "%d.%d.%d.%d:%d", ip >> 24 & 0xFF, ip >> 16 & 0xFF,
            ip >> 8 & 0xFF, ip & 0xFF, (Uint16)port);
}
}

Server::Server() {
    SDL_version compile_version;
    const SDL_version *link_version = SDLNet_Linked_Version();
    SDL_NET_VERSION(&compile_version);

    memset(buffer, 0, 1024);

    print("\n[INFO] Compiled with SDL_net version: {:d}.{:d}.{:d}\n",
          compile_version.major, compile_version.minor, compile_version.patch);

    print("[INFO] Running with SDL_net version: {:d}.{:d}.{:d}\n\n",
          link_version->major, link_version->minor, link_version->patch);

    print("-- INITIALIZING --\n");

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

    if (SDLNet_ResolveHost(&ip, NULL, PORT_NUMBER) != 0) {
        fprintf(stderr, "[ERROR] SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    if (!(socket = SDLNet_TCP_Open(&ip))) {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Listening on port %d...\n", PORT_NUMBER);
}

Server::~Server() { printf("[INFO] Server shut down.\n\n"); }

int Server::exec() {
    char host_str[22];
    while (true) {
        if ((clientsocket = SDLNet_TCP_Accept(socket))) {
            if ((remoteIP = SDLNet_TCP_GetPeerAddress(clientsocket))) {

                hoststr(host_str, remoteIP);
                printf("[INFO] Host connected: %s\n", host_str);
            } else {
                fprintf(stderr, "[ERROR] SDLNet_TCP_GetPeerAddress: %s\n",
                        SDL_GetError());
                continue;
            }

            while (true) {
                int bytes_read = SDLNet_TCP_Recv(clientsocket, buffer, 1024);
                if (bytes_read > 0) {

                    printf("[INFO] (%s) says: (%d) %s\n", host_str, bytes_read,
                           buffer);

                    if (bytes_read == 1) {
                        // The client sent us their protocol version.
                        // Disconnect if it is not the correct one.
                        if (buffer[0] != PROTOCOL_VERSION) {
                            printf("[INFO] Incorrect protocol version\n");
                            break;
                        }
                    } else {
                        // Parse as JSON.
                        std::string err;
                        auto json = json11::Json::parse(buffer, err);

                        if (json == json11::Json()) {
                            print("[ERROR] Failed to parse JSON: {}", err);
                        }

                        auto type = json["type"].dump();

                        print("[INFO] Message Type: {}\n", type);
                    }

                } else {
                    break;
                }
                // Clean the buffer.
                memset(buffer, 0, 1024);
            }

            printf("[INFO] Client (%s) disconnected.\n", host_str);
            SDLNet_TCP_Close(clientsocket);
        }
    }
}
