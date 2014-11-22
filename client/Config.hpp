#pragma once

#include <string>

namespace client {

/// @brief The configuration structure that holds
///  different config variables.
struct Config {
    std::string host = "localhost";
    int port = 4544;

    std::string name = "SneakySnake";
};
} // namespace client
