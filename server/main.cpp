#include <SDL_net.h>
#include <climits>
#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>

#include "lib/Server.hpp"

#include "lib/util.hpp"

#define PORT_NUMBER 4544 // The default port number.

int main(int argc, char **argv) {
    IPaddress address;

    // We could also load from a configuration file
    // here. This would be done after this variable
    // is assigned to PORT_NUMBER.
    int port = PORT_NUMBER;

    bool map_given = false;
    std::string map_name;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--port")) {
            if (i == argc - 1) {
                printf("SERVER: [ERR]  Argument must be supplied after"
                       " `--port`.\n");
                exit(1);
            }
            int temp_port = strtol(argv[i + 1], NULL, 10);
            if (temp_port < 1 || temp_port > 65535) {
                printf("SERVER: [ERR]  Invalid port! Must be between 1 and "
                       "65535.\n");
                exit(1);
            } else {
                port = temp_port;
            }
            i++;
        } else if (!strcmp(argv[i], "--map")) {
            if (i == argc - 1) {
                printf("SERVER: [ERR]  Nothing given for map.\n");
                exit(1);
            }
            map_name = argv[i + 1];
            i++;
            map_given = true;
        }
    }

    // How could we run the server if we had no map?
    if (!map_given) {
        printf("SERVER: [ERR]  No map given. I'm going to close my self "
               "now.\n");
        exit(1);
    }

    std::ifstream map_file(map_name);

    if (!map_file.is_open()) {
        printf("SERVER: [ERR]  Looks like that map file doesn't exist.\n");
        map_file.close();
        exit(1);
    } else {

#ifdef _WIN32
        struct _stat st;
        _stat(map_name.c_str(), &st);
        if (st.st_mode & _S_IFDIR) {
#else
        struct stat st;
        stat(map_name.c_str(), &st);
        if (st.st_mode & S_IFDIR) {
#endif
            printf("SERVER: [ERR]  I need a map FILE, silly, not a "
                   "folder.\n");
            map_file.close();
            exit(1);
        }
        printf("SERVER: [INFO] Map set to '%s'\n", map_name.c_str());
    }
    map_file.close();

    if (SDLNet_ResolveHost(&address, nullptr, port) < 0) {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    server::Server server(address, 2, map_name);
    server.exec();
}
