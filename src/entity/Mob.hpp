#pragma once
#include "entity/Entity.hpp"

#include <string>

namespace mob {

typedef enum {
    NORTH,
    SOUTH,
    WEST,
    EAST
} Direction;

/// @brief Return the string name of a direction.
///
/// @param d The direction
std::string directionName(Direction d);
}

using namespace mob;

class Mob : public Entity {

public:
    Mob(float x, float y, float speed = 1.0f, Direction d = SOUTH);
    int getHealth();
    void setHealth(int health);
    Direction getDirection();
    void setDirection(Direction d);

protected:
    int m_health = 0;
    float m_speed;
    Direction m_direction;
};
