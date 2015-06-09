#include <cstdlib>
#include <fstream>

#include "format.h"

#include <sys/types.h>
#include <sys/stat.h>

#include "lib/Server.hpp"
#include "lib/Config.hpp"

#include "common/util/fileutil.hpp"

#define PORT_NUMBER 4544 // The default port number.

std::string help =
"HELP:\n"
"  --map  <file>       : Initial level to load\n"
"  --port <port>       : What port to run on (default 4544)\n"
"  --no-downloads      : Disable clients downloading content\n"
"  --max-clients <max> : Maximum clients allowed on server (default 5)\n"
"  --resource          : Path to server resource package\n";


int main(int argc, char ** argv) {
    // We could also load from a configuration file
    // here. This would be done after this variable
    // is assigned to PORT_NUMBER.
    server::Config config;

    config.allow_downloads = true;
    config.max_clients = 5;
    config.port = PORT_NUMBER;
    config.resource_package = "resources.tar";

    bool map_given = false;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--help")) {
            fmt::print(help);
            exit(0);
        }
        if (!strcmp(argv[i], "--port")) {
            if (i == argc - 1) {
                fmt::print("SERVER: [ERR]  Argument must be supplied after"
                           " `--port`.\n");
                exit(1);
            }
            int temp_port = strtol(argv[i + 1], nullptr, 10);
            if (temp_port < 1 || temp_port > 65535) {
                fmt::print("SERVER: [ERR]  Invalid port! Must be between 1 and "
                           "65535.\n");
                exit(1);
            } else {
                config.port = temp_port;
            }
            i++;
        } else if (!strcmp(argv[i], "--map")) {
            if (i == argc - 1) {
                fmt::print("SERVER: [ERR]  Nothing given for map.\n");
                exit(1);
            }
            config.map = argv[i + 1];
            i++;
            map_given = true;
        } else if (!strcmp(argv[i], "--no-downloads")) {
            config.allow_downloads = false;
        } else if (!strcmp(argv[i], "--max-clients")) {
            if (i != argc - 1) {
                int temp_max = strtol(argv[i + 1], nullptr, 10);

                if (temp_max < 1) {
                    fmt::print("SERVER: [ERR]  Max clients must be > 0\n");
                    exit(1);
                } else {
                    config.max_clients = temp_max;
                }
            }
        } else if (!strcmp(argv[i], "--resources")) {
            if (i == argc - 1) {
                fmt::print("SERVER: [ERR]  Nothing given for resource "
                           "package.\n");
                exit(1);
            }
            config.resource_package = argv[i + 1];
            i++;
        }
    }

    // How could we run the server if we had no map?
    if (!map_given) {
        fmt::print("SERVER: [ERR]  No map given. I'm going to close my self "
                   "now.\n");
        exit(1);
    }

    std::ifstream map_file(config.map);
    std::ifstream resource_package(config.resource_package);

    if (!map_file.is_open()) {
        fmt::print("SERVER: [ERR]  Failed opening map file \"{}\".\n",
                   config.map);
        map_file.close();
        exit(1);
    } else {
        if (common::util::file::isDirectory(config.map)) {
            fmt::print("SERVER: [ERR]  Directory given for level path, "
                       "expected file.");
            map_file.close();
            exit(1);
        }
        fmt::print("SERVER: [INFO] Map set to '{}'\n", config.map.c_str());
    }
    map_file.close();

    if (!resource_package.is_open()) {
        fmt::print("SERVER: [ERR]  Failed to open resource package \"{}\"\n",
                   config.resource_package);
        resource_package.close();
        exit(1);
    } else {
        if (common::util::file::isDirectory(config.resource_package)) {
            fmt::print("SERVER: [ERR]  Resource package must be a tar file, "
                       "not a directory.\n");

            resource_package.close();
            exit(1);
        }
    }
    resource_package.close();

    server::Server server(config);
    server.exec();
}
