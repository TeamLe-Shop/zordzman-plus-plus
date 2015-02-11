#ifdef __ENTITIES__

#pragma once

#include "Mob.hpp"

namespace client {
namespace mob {

class Eyenado : public Mob {
public:
    Eyenado(float x, float y);
    void render() const override;
    Eyenado * clone() const override;

private:
    void tick() override;
    /// Called when eyenado attempts to attack something.
    void attack();
    // Used for animation.
    int ticks = 0;
};

} // namespace mob
} // namespace client

#endif
