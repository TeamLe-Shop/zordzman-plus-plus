#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "common/entity/component.hpp"

#define COMPONENT(ent, type, as) auto as = \
    static_cast<type *>(ent[type::getComponentName()])

namespace entity {

// Entity ID, component name, field name, new value as JSON
typedef std::tuple<unsigned int,
                   std::string, std::string, json11::Json> StateChange;
typedef std::tuple<std::string, std::string, json11::Json> EntityStateChange;


class Entity {

public:
    Entity(unsigned int id) {
        m_id = id;
    }

    unsigned int getID() {
        return m_id;
    }

    void addComponent(Component *component) {
        m_components[component->getName()].reset(component);
    }

    bool hasComponent(std::string name) {
        return m_components.count(name) == 1;
    }

    std::vector<EntityStateChange> collectStateChanges() {
        std::vector<EntityStateChange> changes;
        for (auto &pair : m_components) {
            for (auto &state_change : pair.second->collectStateChanges()) {
                changes.emplace_back(pair.first,
                                     std::get<0>(state_change),
                                     std::get<1>(state_change));
            }
        }
        return changes;
    }

    Component * operator[](std::string name) {
        return m_components[name].get();
    }

private:
    unsigned int m_id;
    std::map<std::string, std::unique_ptr<Component>> m_components;
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

    void addSystem(System system,
                   std::initializer_list<std::string> components) {
        m_systems.emplace_back(system, components);
    }

    void addSystem(System system) {
        addSystem(system, {});
    }

    void addSystem(SimpleSystem system,
                   std::initializer_list<std::string> components) {
        addSystem([system](EntityCollection *c, Entity &e) {
            return system(e);
        }, components);
    }

    void addSystem(SimpleSystem system) {
        addSystem(system, {});
    }

    std::vector<StateChange> collectStateChanges() {
        std::vector<StateChange> changes;
        for (auto &entity : m_entities) {
            for (auto &entity_changes : entity.collectStateChanges()) {
                changes.emplace_back(entity.getID(),
                                     std::get<0>(entity_changes),
                                     std::get<1>(entity_changes),
                                     std::get<2>(entity_changes));
            }
        }
        return changes;
    }

    void cycle() {
        // Of course currently this is horribly inefficient. The hope is that
        // Systems can specify their required components when they register
        // with the collection so the collection can optimise the interation.
        for (auto &system_config : m_systems) {
            auto system = std::get<0>(system_config);
            auto components = std::get<1>(system_config);
            for (auto &entity : m_entities) {
                bool apply_system = true;
                for (auto &component : components) {
                    apply_system =
                        apply_system && entity.hasComponent(component);
                }
                if (apply_system) {
                    system(this, entity);
                }
            }
        }
        m_frame++;
    }

private:
    unsigned int m_entity_id;
    unsigned int m_frame;
    std::vector<Entity> m_entities;
    std::vector<std::tuple<System, std::vector<std::string>>> m_systems;
};


}  // namespace entity
