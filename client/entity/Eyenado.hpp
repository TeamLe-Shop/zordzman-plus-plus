#pragma once

class Eyenado : public Mob {
public:
    Eyenado(float x, float y);
    void render();
    
private:
    void tick();
    void attack();
    int ticks = 0;
};
