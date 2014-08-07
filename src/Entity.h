#pragma once
#include "Sprite.hpp"

class Entity {
    public:
    int x;
    int y;
    int w;
    int h;
    virtual void render();
    private:
    Sprite spr;
};