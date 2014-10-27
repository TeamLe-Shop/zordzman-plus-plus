#pragma once

#include <SDL_net.h>

namespace client {
namespace net {

char PROTOCOL_VERSION = 0;
Uint32 MAGIC_NUMBER = 0xCAC35500 | PROTOCOL_VERSION;

} // namespace net
} // namespace client
