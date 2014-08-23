
#include <SDL_net.h>

#include "lib/Server.hpp"


int main() {
    IPaddress address = {INADDR_ANY, 9001};
    Server server(&address);
    server.exec();
}
