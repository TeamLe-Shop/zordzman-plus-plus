#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
    Player(float nx, float ny);
    void render();
};