#include "TitleState.hpp"

#include "gfx/drawingOperations.hpp"
#include "gfx/hud/Header.hpp"

#include "audioPlayer.hpp"

#include "Client.hpp"

namespace client {

namespace state {

using namespace drawingOperations;

TitleState::TitleState() {
    audio::playSound("woah");
}

void TitleState::render(client::Client * c) {
    glColor3f(1, 1, 1);

    int width = c->m_window.getWidth();

    std::string maintitle = "Zordzman!";
    Color headercolor, titlecolor;
    headercolor.setInt(225, 165, 105, 255);
    titlecolor.setInt(255, 219, 25, 255);
    gfx::Header header(0, 0, width, 114, 14, headercolor, titlecolor, maintitle,
                       48);
    header.render();

    glColor4f(1, 1, 1, 1);

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 25; j++) {
            drawSprite("grass", j * 32, 128 + (i * 32), 32, 32, 0);
        }
    }

    drawingOperations::drawSprite("selector", 650, 42, 64, 32, tick % 360);
    drawingOperations::drawText("default", "v0.1", 0, 584, 16, 16);

    tick++;
}

void TitleState::input() {
}

} // namespace state

} // namespace client
