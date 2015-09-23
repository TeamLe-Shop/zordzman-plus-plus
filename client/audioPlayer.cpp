#include "audioPlayer.hpp"

#include "ResourceManager.hpp"
#include "common/resources/MusicResource.hpp"
#include "format.h"
#include "Game.hpp"

#include <iterator>

#include <SDL_mixer.h>

namespace client {
namespace audio {

void playMusic(std::string name) {
    ResourceManager * manager = &Game::get().m_resources;

    MusicResource music = manager->m_music[name];

    if (!music.m_valid) {
        throw std::runtime_error(fmt::format("Music resource {} not found!",
                                             name));
        return;
    }

    Mix_PlayMusic(manager->getMusic(music.m_path), -1);
}

void playSound(std::string name) {
    ResourceManager * manager = &Game::get().m_resources;

    SoundResource sound = manager->m_sounds[name];

    if (!sound.m_valid) {
        throw std::runtime_error(fmt::format("Sound resource {} not found!",
                                             name));
        return;
    }

    Mix_PlayChannel(-1, manager->getSound(sound.m_path), 0);
}


} // namespace audio
} // namespace client
