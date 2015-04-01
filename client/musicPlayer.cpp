#include "musicPlayer.hpp"

#include "ResourceManager.hpp"
#include "common/resources/MusicResource.hpp"
#include "format.h"
#include "Client.hpp"

#include <iterator>

#include <SDL_mixer.h>

namespace client {
namespace music {

void playMusic(std::string name) {
    ResourceManager * manager = &Client::get().m_resources;

    MusicResource music = manager->m_music[name];

    if (!music.m_valid) {
        throw std::runtime_error(fmt::format("Music resource {} not found!",
                                             name));
        return;
    }

    Mix_PlayMusic(manager->getMusic(music.m_path.c_str()), -1);
}

} // namespace music
} // namespace client
