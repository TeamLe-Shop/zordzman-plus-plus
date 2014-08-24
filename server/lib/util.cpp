
#include "util.hpp"

std::string IPaddress_AsString(IPaddress *address) {
    return fmt::format("{}.{}.{}.{}:{}", address->host >> 24 & 0xFF,
                       address->host >> 16 & 0xFF, address->host >> 8 & 0xFF,
                       address->host >> 0 & 0xFF, address->port);
}
