#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
    Player(float nx, float ny);
    void render();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    float speed;
};