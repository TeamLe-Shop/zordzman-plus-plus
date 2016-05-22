#include "TitleState.hpp"

#include "gfx/drawingOperations.hpp"
#include "gfx/hud/Header.hpp"
#include "gfx/hud/Button.hpp"

#include "audioPlayer.hpp"
#include "Client.hpp"
#include "language.hpp"

#include <cmath>

namespace client {

using namespace drawingOperations;
using namespace language;

TitleState::TitleState() {
    audio::playSound("woah");
}

void TitleState::render(client::Client * c) {
    glColor3f(1, 1, 1);

    int width = c->m_window.getWidth();
    int height = c->m_window.getHeight();

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

    Color box(0.2, 0.2, 0.2, 1);
    Color box_outline(0.9, 0.9, 0.9, 1);

    drawingOperations::drawSprite("easteregg", width/2-32, 114, 64, 64,
        sinf((tick%360) * (M_PI/180)) * 30.0, Hinge(width/2, 114));
    drawingOperations::drawText("default", "v0.0.9", 0, 584, 16, 16);

    drawingOperations::setColor(box);
    drawingOperations::drawRectangle(width/2-175, 200, 350, height-250, true);
    drawingOperations::setColor(box_outline);
    drawingOperations::drawRectangle(width/2-175, 200, 350, height-250, false);

    gfx::Button spbutton(width/2-128, 250, 256, 24, translate(Key_Singleplayer),
                         16, Color(0, 0.2, 0.7, 1), nullptr, nullptr);

    gfx::Button mpbutton(width/2-128, 300, 256, 24, translate(Key_Multiplayer),
                         16, Color(0.7, 0, 0, 1), nullptr, nullptr);

    gfx::Button cbutton(width/2-128, 350, 256, 24, translate(Key_Credits), 16,
                       Color(0, 0.7, 0, 1), nullptr, nullptr);

    gfx::Button obutton(width/2-128, 400, 256, 24, translate(Key_Options), 16,
                       Color(0, 0.7, 0.7, 1), nullptr, nullptr);

    gfx::Button qbutton(width/2-128, 450, 256, 24, translate(Key_Quit), 16,
                       Color(0.85, 0.7, 0, 1), nullptr, nullptr);
    spbutton.render();
    mpbutton.render();
    cbutton.render();
    obutton.render();
    qbutton.render();

    tick++;
}

void TitleState::input(SDL_Event event) {

}

} // namespace client
