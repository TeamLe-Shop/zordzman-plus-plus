#pragma once

#include "Mob.hpp"

namespace client {
namespace mob {
class Eyenado : public Mob {
public:
    Eyenado(float x, float y);
    void render();

private:
    void tick();
    void attack();
    int ticks = 0;
};
}
}
