#include "System.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

namespace sys {

System::System() {
    SDL_Init(SDL_INIT_EVERYTHING);
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        std::cerr << "Failed to initialize SDL_image: " << IMG_GetError();
        std::abort();
    }
}

System::~System() {
    IMG_Quit();
    SDL_Quit();
}
}
