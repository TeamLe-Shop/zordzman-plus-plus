#include "System.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <sstream>
#include <stdexcept>

namespace sys {

System::System() {
#define INIT_OR_THROW(cond, msg)                                               \
    if (!(cond)) {                                                             \
        std::ostringstream s;                                                  \
        s << msg;                                                              \
        throw std::runtime_error(s.str());                                     \
    }

    INIT_OR_THROW(SDL_Init(SDL_INIT_EVERYTHING) == 0,
                  "Failed to initialize SDL: " << SDL_GetError());
    INIT_OR_THROW(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG,
                  "Failed to initialize SDL_image: " << IMG_GetError());
    INIT_OR_THROW(SDLNet_Init() != -1,
                  "Failed to initialize SDL_net: " << SDLNet_GetError());

#undef INIT_OR_THROW
}

System::~System() {
    SDLNet_Quit();
    IMG_Quit();
    SDL_Quit();
}
}
