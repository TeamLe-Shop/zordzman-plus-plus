#include "physicsSystem.hpp"

#include "common/entity/components/position.hpp"
#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"

#include "common/util/debug.hpp"

#include <cmath>

using common::util::debug;

const float pi = acos(-1);

void physicsSystem(entity::EntityCollection * coll, entity::Entity & ent) {
    auto position = COMPONENT(ent, entity::PositionComponent);
    auto character = COMPONENT(ent, entity::CharacterComponent);
    auto render = COMPONENT(ent, entity::RenderComponent);

    double x = position->m_x.get();
    double y = position->m_y.get();

    double xz = sinf(((coll->getFrame() / 1000.0) * 180.0) / pi) * 3;
    double yz = cosf(((coll->getFrame() / 1000.0) * 180.0) / pi) * 3;
    double rot = render->m_rotation.get();

    x += xz;
    y += yz;
    rot += 4;


    position->m_x.set(x);
    position->m_y.set(y);
    render->m_rotation.set(rot);

//     debug("Frame #{}, Entity: \"{}\" (#{}), Position: ({}, {})\n",
//           coll->getFrame(), character->m_name.get(), ent.getID(),
//           position->m_x.get(), position->m_y.get());
}
