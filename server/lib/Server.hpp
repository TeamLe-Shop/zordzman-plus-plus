#pragma once

#include <SDL_net.h>

class Server {
public:
    Server();
    ~Server();
    int exec();

private:
    TCPsocket socket, clientsocket;
    IPaddress ip, *remoteIP;
    char buffer[1024];
};
