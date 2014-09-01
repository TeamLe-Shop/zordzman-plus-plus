#pragma once

#include <string>

#include <SDL_net.h>

#include "format.h"

namespace server {
/// @brief Returns an IP address as a string in the format: i.i.i.i:port
///
/// @param address The IP address object (ptr)
std::string IPaddress_AsString(IPaddress *address);
}
