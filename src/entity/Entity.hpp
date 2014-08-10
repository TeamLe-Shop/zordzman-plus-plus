#pragma once

class Entity {
public:
    float x;
    float y;
    float w;
    float h;
    virtual void render();
    virtual void tick();
    virtual ~Entity();
    virtual Entity *clone() const = 0;
};
