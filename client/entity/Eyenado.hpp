#pragma once

#include "Mob.hpp"

namespace client {
namespace mob {
class Eyenado : public Mob {
public:
    Eyenado(float x, float y);
    void render() const;
    Eyenado * clone() const;

private:
    void tick();
    void attack();
    int ticks = 0;
};
}
}
