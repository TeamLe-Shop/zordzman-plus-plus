#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "common/extlib/json11/json11.hpp"

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
    Entity(unsigned int id);

    unsigned int getID();
    void addComponent(Component *component);
    bool hasComponent(std::string name);
    std::vector<EntityStateChange> collectStateChanges();

    Component * operator[](std::string name);

private:
    unsigned int m_id;
    std::map<std::string, std::unique_ptr<Component>> m_components;
};


class EntityCollection {

using System = std::function<void(EntityCollection *, Entity &)>;
using SimpleSystem = std::function<void(Entity &)>;

public:
    EntityCollection();

    unsigned int getFrame();
    Entity & createEntity();
    void addSystem(System system,
                   std::initializer_list<std::string> components);
    void addSystem(System system);
    void addSystem(SimpleSystem system,
                   std::initializer_list<std::string> components);
    void addSystem(SimpleSystem system);
    void registerComponent(std::string name,
                           std::function<Component *()> allocator);
    std::vector<StateChange> collectStateChanges();
    void handleEntityStateChange(json11::Json entity);
    void cycle();

private:
    unsigned int m_entity_id;
    unsigned int m_frame;
    std::vector<Entity> m_entities;
    std::vector<std::tuple<System, std::vector<std::string>>> m_systems;
    std::map<std::string, std::function<Component *()>> m_component_types;

    Entity & createEntity(unsigned int id);
};


}  // namespace entity
