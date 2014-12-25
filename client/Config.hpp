#pragma once

#include <string>

namespace client {

/// The configuration structure that holds
///  different config variables.
struct Config {
    std::string host = "localhost"; // localhost doesn't work
                                    // IPv6 not supported.
    int port = 4544;

    std::string name = "SneakySnake";
};
} // namespace client
