#include <SDL_net.h>

#include "lib/Server.hpp"

int main() {
    IPaddress address;

    if (SDLNet_ResolveHost(&address, nullptr, 9001) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    server::Server server(&address, 2);
    server.exec();
}
