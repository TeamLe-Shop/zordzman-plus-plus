#pragma once

#include <functional>
#include <string>
#include <vector>

#include "common/entity/component.hpp"

#define COMPONENT(ent, type, as) auto as = \
    static_cast<type *>(ent[type::getComponentName()])

namespace entity {

class Entity {

public:
    Entity(unsigned int id) {
        m_id = id;
    }

    ~Entity() {
        for (auto pair : m_components) {
            delete std::get<1>(pair);
        }
    }

    unsigned int getID() {
        return m_id;
    }

    void addComponent(Component *component) {
        m_components[component->getName()] = component;
    }

    bool hasComponent(std::string name) {
        return m_components.count(name) == 1;
    }

    Component * operator[](std::string name) {
        return m_components[name];
    }

private:
    unsigned int m_id;
    std::map<std::string, Component *> m_components;
};


class EntityCollection {

using System = std::function<void(EntityCollection *, Entity &)>;
using SimpleSystem = std::function<void(Entity &)>;

public:
    EntityCollection() {
        m_entity_id = 0;
        m_frame = 0;
    }

    unsigned int getFrame() {
        return m_frame;
    };

    Entity & createEntity() {
        m_entities.emplace_back(m_entity_id);
        m_entity_id++;
        return m_entities.back();
    }

    void addSystem(System system) {
        m_systems.push_back(system);
    }

    void addSystem(SimpleSystem system) {
        addSystem([system](EntityCollection *c, Entity &e) {
            return system(e);
        });
    }

    void cycle() {
        // Of course currently this is horribly inefficient. The hope is that
        // Systems can specify their required components when they register
        // with the collection so the collection can optimise the interation.
        for (auto &system : m_systems) {
            for (auto &entity : m_entities) {;
                system(this, entity);
            }
        }
        m_frame++;
    }

private:
    unsigned int m_entity_id;
    unsigned int m_frame;
    std::vector<Entity> m_entities;
    std::vector<System> m_systems;
};


}  // namespace entity
