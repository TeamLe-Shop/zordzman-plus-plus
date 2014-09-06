#include <SDL_net.h>

#include "lib/Server.hpp"

#define PORT_NUMBER 4544 // The default port number.

int main() {
    IPaddress address;

    // We could also load from a configuration file
    // here. This would be done after this variable
    // is assigned to PORT_NUMBER.
    int port = PORT_NUMBER;

    if (SDLNet_ResolveHost(&address, nullptr, port) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    server::Server server(address, 2);
    server.exec();
}
