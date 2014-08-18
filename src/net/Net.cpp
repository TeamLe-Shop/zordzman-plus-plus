#include "net/Net.hpp"

namespace net {

char const PROTOCOL_VERSION = 0;

bool initNet() {
    // Quit if we couldn't initialize SDLNet.
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "[ERROR] SDLNet_Init: %s\n", SDLNet_GetError());
        fprintf(stderr,
                "[ERROR] Failed to initialize SDLNet. Quitting zordzman...\n");
        return false;
    }
    return true;
}

void cleanUp() {
    SDLNet_Quit();
}

} //namespace net