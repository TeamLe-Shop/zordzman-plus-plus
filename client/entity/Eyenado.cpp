#include "Eyenado.hpp"
#include "gfx/drawingOperations.hpp"
#include "Mob.hpp"
#include "Game.hpp"

using namespace mob;

Eyenado::Eyenado(float x, float y) {
    : Mob(x, y, 1.8f, DOWN) {
    m_health = 45;
}

void Eyenado::render() {
    // i'm currently using Player.cpp as a drawing reference
    SpriteSheet &sheet = Game::get().resources.getSheet("main");
    sheet.setSpriteSize(16);
    // spin animation
    if (ticks >= 0 && ticks < 30) {
        drawSpriteFromSheet(sheet, 0, 4, m_x, m_y 32, 32);
    } else if (ticks >= 30 && ticks < 60) {
        drawSpriteFromSheet(sheet, 1, 4, m_x, m_y, 32, 32);
    } else if (ticks >= 60 && ticks < 90) {
        drawSpriteFromSheet(sheet, 2, 4, m_x, m_y, 32, 32);
    } else if (ticks > 90) {
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
