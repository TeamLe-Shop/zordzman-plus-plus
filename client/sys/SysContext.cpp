#include "SysContext.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
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
} // Anonymous namespace

SysContext::SysContext() {
    int const AUDIO_RATE = 44100;
    Uint16 const AUDIO_FORMAT = AUDIO_S16SYS;
    int const AUDIO_CHANNELS = 2;
    int const AUDIO_CHUNK_SIZE = 4096;

    try_init(SDL_Init(SDL_INIT_EVERYTHING) == 0, "Failed to initialize SDL: {}",
             SDL_GetError());
    try_init(IMG_Init(IMG_INIT_PNG) == IMG_INIT_PNG,
             "Failed to initialize SDL_image: {}", IMG_GetError());
    try_init(Mix_Init(MIX_INIT_OGG) == MIX_INIT_OGG,
             "Failed to initialize SDL_mixer: {}", Mix_GetError());
    try_init(Mix_OpenAudio(AUDIO_RATE, AUDIO_FORMAT, AUDIO_CHANNELS,
                           AUDIO_CHUNK_SIZE) > -1,
             "Failed to open audio: {}", Mix_GetError());
    try_init(TTF_Init() == 0, "Failed to initialize SDL_ttf: {}",
             TTF_GetError());
}

SysContext::~SysContext() {
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}
} // namespace sys
} // namespace client
