#include "SysContext.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_net.h>
#include <SDL_mixer.h>
#include <stdexcept>

#include "format.h"

namespace client {
namespace sys {

namespace {
template <typename... Args> void try_init(bool cond, Args... args) {
    if (!cond) {
        throw std::runtime_error(fmt::format(args...));
    }
}
}

SysContext::SysContext() {
    int const AUDIO_RATE = 44100;
    Uint16 const AUDIO_FORMAT = AUDIO_S16SYS;
    int const AUDIO_CHANNELS = 2;
    int const AUDIO_CHUNK_SIZE = 4096;

    try_init(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL: {}",
             SDL_GetError());
    try_init(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG,
             "Failed to initialize SDL_image: {}", IMG_GetError());
    try_init(SDLNet_Init() != -1, "Failed to initialize SDL_net: {}",
             SDLNet_GetError());
    try_init(Mix_Init(MIX_INIT_OGG) == MIX_INIT_OGG,
             "Failed to initialize SDL_mixer: {}", Mix_GetError());
    try_init(Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS,
                           AUDIO_CHUNK_SIZE) > -1,
             "Failed to open audio: {}", Mix_GetError());
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
