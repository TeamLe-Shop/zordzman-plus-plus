#include "Entity.hpp"

namespace client {
Entity::Entity(float x, float y) : m_x(x), m_y(y) {}

void Entity::render() const {}

void Entity::tick() {}

Entity::~Entity() {}
}
