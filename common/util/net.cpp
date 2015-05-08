#include "net.hpp"

#include <cstring>

namespace common {
namespace util {
namespace net {

std::string ipaddr(struct sockaddr_in const addr, bool include_port) {
    uint32_t host = ntohl(addr.sin_addr.s_addr);
    uint16_t port = ntohs(addr.sin_port);
    if (include_port) {
        return fmt::format("{}.{}.{}.{}:{}", host >> 24 & 0xFF,
                           host >> 16 & 0xFF, host >> 8 & 0xFF,
                           host >> 0 & 0xFF, port);
    } else {
        return fmt::format("{}.{}.{}.{}", host >> 24 & 0xFF, host >> 16 & 0xFF,
                           host >> 8 & 0xFF, host >> 0 & 0xFF);
    }
}

void resolvehost(struct sockaddr_in & addr, std::string host) {
    // Convert human-readable domain name/ip string
    // to `struct sockaddr_in`.
    addrinfo * result;
    int error;

    addrinfo criteria;
    memset(&criteria, 0, sizeof(criteria));
    criteria.ai_family = AF_INET;
    criteria.ai_protocol = SOCK_STREAM;
    criteria.ai_flags = AI_PASSIVE;

    if ((error =
             getaddrinfo(host.c_str(), nullptr, &criteria, &result))) {
        fmt::print("Error resolving host name: {}\n", gai_strerror(error));
        return;
    }

    memcpy(&addr, result->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(result);
}

} // namespace net
} // namespace util
} // namespace common
