#include "System.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <iostream>
#include <stdexcept>

namespace sys {

System::System() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
        std::abort();
    }
    // Quit if we couldn't initialize SDLNet.
    if (SDLNet_Init() == -1) {
        fprintf(stderr, "[ERROR] SDLNet_Init: %s\n", SDLNet_GetError());
        fprintf(stderr,
                "[ERROR] Failed to initialize SDLNet. Quitting zordzman...\n");
        throw std::runtime_error("Lil");
    }
}

System::~System() {
    SDLNet_Quit();
    IMG_Quit();
    SDL_Quit();
}
}
