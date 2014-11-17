#pragma once

#include <ostream>
#include <string.h>

#define starts(a, b) !memcmp(a, b, strlen(b))

std::ostream &operator<<(std::ostream &os, struct sockaddr const &addr);
