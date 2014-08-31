#include "util.hpp"

namespace server {
std::string IPaddress_AsString(IPaddress *address) {
    Uint32 host = SDLNet_Read32(&address->host);
    Uint16 port = SDLNet_Read16(&address->port);
    return fmt::format("{}.{}.{}.{}:{}", host >> 24 & 0xFF,
                       host >> 16 & 0xFF, host >> 8 & 0xFF,
                       host >> 0 & 0xFF, port);
}
}
