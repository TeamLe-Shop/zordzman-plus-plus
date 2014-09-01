#pragma once

#include <ostream>
#include <SDL_net.h>

std::ostream &operator<<(std::ostream &os, IPaddress const &addr);
