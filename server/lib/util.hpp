#pragma once

#include <ostream>
#include <SDL_net.h>
#include <string.h>

#define starts(a, b) !memcmp(a, b, strlen(b))

std::ostream &operator<<(std::ostream &os, struct sockaddr const &addr);
