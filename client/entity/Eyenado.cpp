#include "Eyenado.hpp"
#include "gfx/drawingOperations.hpp"
#include "Mob.hpp"

using namespace mob;

Eyenado::Eyenado(float x, float y) {
    : Mob(x, y, 1.8f, DOWN) {
    m_health = 45;
}

void Eyenado::render() {}

void Eyenado::tick() {}

void Eyenado::attack() {}
