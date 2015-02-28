#pragma once

#include <string>

namespace client {

/// The configuration structure that holds
///  different config variables.
class Config {

public:
    Config(std::string config);

// You can make this private if you want to add more complications.
public:
    std::string host = "localhost";
    int port = 4544;
    std::string name = "gatsan";
};

} // namespace client
