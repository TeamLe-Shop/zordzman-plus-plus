#include "physicsSystem.hpp"

#include "common/entity/components/position.hpp"

void physicsSystem(entity::EntityCollection * coll,
                                  entity::Entity & ent) {
    COMPONENT(ent, entity::PositionComponent, position);
    fmt::print("Frame #{}, Entity ID #{}: ({}, {})\n",
               coll->getFrame(), ent.getID(), position->m_x.get(),
               position->m_y.get());
}
