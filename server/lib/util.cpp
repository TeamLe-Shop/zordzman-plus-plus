#include "util.hpp"
#include <format.h>

#include <sys/sockets.h>

std::ostream &operator<<(std::ostream &os, struct sockaddr const addr) {
    uint32_t host = ntohl(addr.sin_addr.s_addr);
    uint16_t port = ntohs(addr.sin_port);
    fmt::print(os, "{}.{}.{}.{}:{}", host >> 24 & 0xFF, host >> 16 & 0xFF,
               host >> 8 & 0xFF, host >> 0 & 0xFF, port);
    return os;
}
