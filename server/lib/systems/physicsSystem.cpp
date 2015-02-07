#include "physicsSystem.hpp"

#include "common/entity/components/position.hpp"
#include "common/entity/components/character.hpp"

#include "common/util/debug.hpp"

using common::util::debug;

void physicsSystem(entity::EntityCollection *coll, entity::Entity &ent) {
    COMPONENT(ent, entity::PositionComponent, position);
    COMPONENT(ent, entity::CharacterComponent, character);
    character->m_health.set(character->m_health.get() - 1);
    debug("Frame #{}, Entity: \"{}\" (#{}), Position: ({}, {})\n",
        coll->getFrame(), character->m_name.get(), ent.getID(),
        position->m_x.get(), position->m_y.get());
}
