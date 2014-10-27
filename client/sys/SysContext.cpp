#include "SysContext.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include <stdexcept>

#include "format.h"

namespace client {
namespace sys {

SysContext::SysContext() {
#define INIT_OR_THROW(cond, ...)                                               \
    if (!(cond)) {                                                             \
        throw std::runtime_error(fmt::format(__VA_ARGS__));                    \
    }

    int const AUDIO_RATE = 44100;
    Uint16 const AUDIO_FORMAT = AUDIO_S16SYS;
    int const AUDIO_CHANNELS = 2;
    int const AUDIO_CHUNK_SIZE = 4096;

    INIT_OR_THROW(SDL_Init(SDL_INIT_EVERYTHING) == 0,
                  "Failed to initialize SDL: {}", SDL_GetError());
    INIT_OR_THROW(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG,
                  "Failed to initialize SDL_image: {}", IMG_GetError());
    INIT_OR_THROW(SDLNet_Init() != -1, "Failed to initialize SDL_net: {}",
                  SDLNet_GetError());
    INIT_OR_THROW(Mix_Init(MIX_INIT_OGG) == MIX_INIT_OGG,
                  "Failed to initialize SDL_mixer: {}", Mix_GetError());
    INIT_OR_THROW(Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS,
                                AUDIO_CHUNK_SIZE) > -1,
                  "Failed to open audio: {}", Mix_GetError());

#undef INIT_OR_THROW
}

SysContext::~SysContext() {
    SDLNet_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}
}
}
