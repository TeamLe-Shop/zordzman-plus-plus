#include "Entity.hpp"

Entity::Entity(float x, float y) : m_x(x), m_y(y) {}

void Entity::render() const {}

void Entity::tick() {}

Entity::~Entity() {}
