#pragma once

#include <string>

namespace client {

/// The configuration structure that holds
///  different config variables.
struct Config {
    std::string host = "gatsan.ddns.net";
    int port = 4544;
    std::string name = "gatsan";
};
} // namespace client
