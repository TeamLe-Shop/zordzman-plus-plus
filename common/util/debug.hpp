#pragma once

#include "format.h"

namespace common {
namespace util {

/// Print to stderr and flush the stream, so you can actually
/// fucking see the debugging output
template <typename... Args> void debug(std::string const fmt, Args... args) {
    fmt::print(stderr, fmt, args...);
    std::fflush(stderr);
}
}
}
