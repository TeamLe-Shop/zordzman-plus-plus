#pragma once

namespace server {

struct Config {
    int port;
    unsigned int max_clients;
    std::string map;
    bool allow_downloads;
    std::string resource_package;
};

typedef struct Config Config;

}
