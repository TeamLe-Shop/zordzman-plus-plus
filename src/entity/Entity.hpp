#pragma once

class Entity {
public:
    Entity(float x, float y);
    virtual void render();
    virtual void tick();
    virtual ~Entity();
    virtual Entity *clone() const = 0;

protected:
    float m_x;
    float m_y;
};
