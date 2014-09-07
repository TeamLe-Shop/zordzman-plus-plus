#include "Eyenado.hpp"
#include "gfx/drawingOperations.hpp"
#include "Mob.hpp"
#include "Client.hpp"

namespace client {
namespace mob {

Eyenado::Eyenado(float x, float y) : Mob(x, y, 1.8f, SOUTH) { m_health = 45; }

void Eyenado::render() {
    // i'm currently using Player.cpp as a drawing reference
    SpriteSheet & sheet = Client::get().resources.getSheet("main");
    sheet.setSpriteSize(16);
    using drawingOperations::drawSpriteFromSheet;
    // spin animation USE column 4
    if (ticks >= 0 && ticks < 30) {
        drawSpriteFromSheet(sheet, 0, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 30 && ticks < 60) {
        drawSpriteFromSheet(sheet, 1, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 60 && ticks < 90) {
        drawSpriteFromSheet(sheet, 2, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 90 && ticks < 120) {
        drawSpriteFromSheet(sheet, 3, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 120 && ticks < 150) {
        drawSpriteFromSheet(sheet, 4, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 180 && ticks < 210) {
        drawSpriteFromSheet(sheet, 5, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 240) {
        ticks = 0;
    }
    // AI
    tick();
    // reset
    sheet.setSpriteSize(8);
    // increase tick, for the animation
    ticks++;
}

void Eyenado::tick() {}

void Eyenado::attack() {}
}
}
