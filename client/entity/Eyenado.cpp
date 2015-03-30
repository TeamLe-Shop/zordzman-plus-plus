#include "Eyenado.hpp"
#include "gfx/drawingOperations.hpp"
#include "Mob.hpp"
#include "Client.hpp"

namespace client {
namespace mob {

Eyenado::Eyenado(float x, float y) : Mob(x, y, 1.8f, SOUTH) { m_health = 45; }

void Eyenado::render() const {
    // Calculate frame
    int idx = 0;
    if (ticks >= 0 && ticks < 15) {
        idx = 0;
    } else if (ticks >= 15 && ticks < 30) {
        idx = 1;
    } else if (ticks >= 30 && ticks < 45) {
        idx = 2;
    } else if (ticks >= 45 && ticks < 60) {
        idx = 3;
    } else if (ticks >= 60 && ticks < 75) {
        idx = 4;
    } else if (ticks >= 75 && ticks < 90) {
        idx = 5;
    }

    // Draw it
   //  sys::Texture & texture = Client::get().m_resources.getTexture("main");
//     using drawingOperations::drawSpriteFromTexture;
//     drawSpriteFromTexture(texture, idx, 4, m_x, m_y, 32, 32, 16);
}

Eyenado * Eyenado::clone() const { return new Eyenado(*this); }

void Eyenado::tick() {
    // increase tick, for the animation
    ticks++;
    if (ticks >= 90) {
        ticks = 0;
    }
    m_x += 0.1;
    m_y += 0.1;
}

void Eyenado::attack() {}

} // namespace mob
} // namespace client
