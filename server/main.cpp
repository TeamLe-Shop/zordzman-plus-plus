#include <SDL_net.h>
#include <climits>
#include <cerrno>
#include <cstdlib>

#include "lib/Server.hpp"

#include "lib/util.hpp"

#define PORT_NUMBER 4544 // The default port number.

int main(int argc, char **argv) {
    IPaddress address;

    // We could also load from a configuration file
    // here. This would be done after this variable
    // is assigned to PORT_NUMBER.
    int port = PORT_NUMBER;

    for (int i = 1; i < argc; i++) {
        if (starts(argv[i], "port=")) {
            int temp_port = strtol(argv[i] + 5, NULL, 10);
            if (temp_port < 1 || temp_port > 65535) {
                printf("Invalid port! Must be between 1 and 65535.\n");
                exit(1);
            } else {
                port = temp_port;
            }
        } else if (starts(argv[i], "map=")) {
            printf("Map set to '%s'\n", argv[i] + 4);
        }
    }

    if (SDLNet_ResolveHost(&address, nullptr, port) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    server::Server server(address, 2);
    server.exec();
}
