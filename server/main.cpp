
#include <SDL_net.h>

#include "lib/Server.hpp"

int main() {
    IPaddress address = { INADDR_ANY, 9001 };
    server::Server server(&address, 1);
    server.exec();
}
