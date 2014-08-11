#pragma once

#include "Entity.hpp"
#include "Mob.hpp"

using namespace mob;

class Player : public Mob {
public:
    Player(float x, float y, float speed = 1.0f);
    void render();
    void tick();
    Player *clone() const;

private:
    void input();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
};
