#include "net.hpp"

namespace common {
namespace util {

std::string ipaddr(struct sockaddr const addr) {
    uint32_t host = ntohl(addr.sin_addr.s_addr);
    uint16_t port = ntohs(addr.sin_port);
    return fmt::format("{}.{}.{}.{}:{}", host >> 24 & 0xFF, host >> 16 & 0xFF,
               host >> 8 & 0xFF, host >> 0 & 0xFF, port);
}

} // namespace util
} // namespace common