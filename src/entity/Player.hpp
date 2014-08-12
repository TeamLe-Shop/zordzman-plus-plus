#pragma once

#include "Entity.hpp"
#include "Mob.hpp"

using namespace mob;

class Player : public Mob {
public:
    /// @brief Initialize the player with x, y, and speed.
    ///
    /// @param x X position
    /// @param y Y Position
    /// @speed The speed, by default 1.0f.
    Player(float x, float y, float speed = 1.0f);
    /// @brief Render the player.
    void render();
    /// @brief 'tick' the player.
    void tick();
    Player *clone() const;

private:
    void input();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
};
