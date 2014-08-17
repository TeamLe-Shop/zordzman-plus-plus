#pragma once

#define PORT_NUMBER 4544

#include <SDL.h>
#include <SDL_net.h>
#include <string>

namespace net {

bool initNet();

void cleanUp();

class TCPSock {
public:
    bool connectToHost(std::string host, int portnum);
    void startReading();
    bool send(std::string buf);
    void close();
private:
    IPaddress m_server;
    TCPsocket m_socket;
};

} //namespace net