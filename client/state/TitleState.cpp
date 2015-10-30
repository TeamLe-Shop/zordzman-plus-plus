#include "TitleState.hpp"

#include "gfx/drawingOperations.hpp"

#include "audioPlayer.hpp"

#include "Client.hpp"

namespace client {

namespace state {

TitleState::TitleState() {
    audio::playMusic("March");
}

void TitleState::render(client::Client * c) {
//    std::string maintitle = "Zordzman";
//    int maintitle_size = 32;
//    int maintitle_x = c->m_window.getWidth() - maintitle.size() / 2;
//     drawingOperations::drawText("default", "Zordzman", maintitle_x, 200,
//                                 maintitle_size, maintitle_size);
//     drawingOperations::drawSprite("grass", 400, 400, 64, 64, 0);
}

void TitleState::input() {
}

} // namespace state

} // namespace client
