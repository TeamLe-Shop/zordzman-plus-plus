#include "Mob.hpp"

#include <string>

namespace mob {

std::string directionName(Direction d) {
    switch (d) {
    case NORTH:
        return std::string("North");
    case SOUTH:
        return std::string("South");
    case WEST:
        return std::string("West");
    case EAST:
        return std::string("East");
    default:
        return std::string("Invalid");
    }
}
}

using namespace mob;

Mob::Mob(float x, float y, float speed, Direction d)
    : Entity(x, y), m_speed(speed), m_direction(d) {}

int Mob::getHealth() { return m_health; }

void Mob::setHealth(int health) { m_health = health; }

Direction Mob::getDirection() { return m_direction; }

void Mob::setDirection(Direction d) { m_direction = d; }
