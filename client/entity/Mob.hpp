#pragma once
#include "entity/Entity.hpp"

#include <string>

namespace client {
namespace mob {

/// @brief A direction enum for cardinal directions.
enum Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

/// @brief Return the string name of a direction.
///
/// @param d The direction
std::string directionName(Direction d);

class Mob : public Entity {

public:
    /// @brief Initialize and set the mob's x, y, speed and direction.
    ///
    /// @param x The x position
    /// @param y The y position
    /// @param speed The speed of the mob, by default 1.0f.
    /// @param d The direction, by default it is SOUTH.
    Mob(float x, float y, float speed = 1.0f, Direction d = SOUTH);
    /// @brief Return the current health
    int getHealth();
    /// @brief Set the current health.
    ///
    /// @param health The health
    void setHealth(int health);
    /// @brief Return the direction of the mob.
    Direction getDirection();
    /// @brief Set the direction of the mob.
    ///
    /// @param d Direction to set it to
    void setDirection(Direction d);

protected:
    // The mob's health.
    int m_health = 0;
    // The mob's speed.
    float m_speed;
    // The direction the mob is facing.
    Direction m_direction;
};
} // namespace mob
} // namespace client
