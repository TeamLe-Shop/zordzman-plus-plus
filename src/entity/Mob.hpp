#pragma once
#include "entity/Entity.hpp"

#include <string>

namespace mob {

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

std::string directionName(Direction d);
}

using namespace mob;

class Mob : public Entity {

public:
    Mob(float x, float y, float speed = 1.0f, Direction d = DOWN);
    int getHealth();
    void setHealth(int health);
    Direction getDirection();
    void setDirection(Direction d);

protected:
    int m_health;
    float m_speed;
    Direction m_direction;
};
