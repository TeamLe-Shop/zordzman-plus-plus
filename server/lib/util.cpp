#include "util.hpp"
#include <format.h>

std::ostream &operator<<(std::ostream &os, IPaddress const &addr) {
    Uint32 host = SDLNet_Read32(&addr.host);
    Uint16 port = SDLNet_Read16(&addr.port);
    fmt::print(os, "{}.{}.{}.{}:{}", host >> 24 & 0xFF, host >> 16 & 0xFF,
               host >> 8 & 0xFF, host >> 0 & 0xFF, port);
    return os;
}
