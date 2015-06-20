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
    std::string host = "127.0.0.1";
    int port = 4544;
    std::string name = "gatsan";
    std::string level_dir = "levels";
};

} // namespace client
