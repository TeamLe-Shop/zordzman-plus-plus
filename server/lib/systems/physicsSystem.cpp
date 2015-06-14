#include "physicsSystem.hpp"

#include "common/entity/components/position.hpp"
#include "common/entity/components/character.hpp"

#include "common/util/debug.hpp"

using common::util::debug;

void physicsSystem(entity::EntityCollection * coll, entity::Entity & ent) {
    auto position = COMPONENT(ent, entity::PositionComponent);
    auto character = COMPONENT(ent, entity::CharacterComponent);

    double minX = 96;
    double maxX = 160;
    double speed = 0.5;

    bool down = false;

    double x = position->m_x.get();
    double y = position->m_y.get();

    if (coll->getFrame() % 360 >= 180) {
        down = false;
    } else {
        down = true;
    }

    if (down) {
        y += speed;
    } else {
        y -= speed;
    }

    position->m_x.set(x);
    position->m_y.set(y);

    debug("Frame #{}, Entity: \"{}\" (#{}), Position: ({}, {})\n",
          coll->getFrame(), character->m_name.get(), ent.getID(),
          position->m_x.get(), position->m_y.get());
}
