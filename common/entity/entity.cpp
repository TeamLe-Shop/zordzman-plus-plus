#include "common/entity/entity.hpp"

namespace entity {

Entity::Entity(unsigned int id) { m_id = id; }

unsigned int Entity::getID() const { return m_id; }

void Entity::addComponent(Component * component) {
    m_components[component->getName()].reset(component);
}

bool Entity::hasComponent(std::string name) const {
    return m_components.count(name) == 1;
}

std::vector<EntityStateChange> Entity::collectStateChanges() {
    std::vector<EntityStateChange> changes;
    for (auto & pair : m_components) {
        for (auto & state_change : pair.second->collectStateChanges()) {
            changes.emplace_back(pair.first, std::get<0>(state_change),
                                 std::get<1>(state_change));
        }
    }
    return changes;
}

Component * Entity::operator[](std::string name) {
    return m_components[name].get();
}

EntityCollection::EntityCollection() {
    m_entity_id = 0;
    m_frame = 0;
}

unsigned int EntityCollection::getFrame() { return m_frame; }

Entity & EntityCollection::createEntity() {
    m_entities.emplace_back(m_entity_id);
    m_entity_id++;
    return m_entities.back();
}

void EntityCollection::removeEntity(unsigned int id) {
    int index = 0;
    for (auto const & e : m_entities) {
        if (e.getID() == id) {
            m_entities.erase(m_entities.begin() + index);
        }
        index++;
    }
}

void EntityCollection::addSystemWithComponents(
    System system, std::initializer_list<std::string> components) {
    m_systems.emplace_back(system, components);
}

void EntityCollection::addSystem(System system) {
    addSystemWithComponents(system, {});
}

void EntityCollection::addSimpleSystemWithComponents(
    SimpleSystem system, std::initializer_list<std::string> components) {
    addSystemWithComponents(
        [system](EntityCollection * /*c*/, Entity & e) { return system(e); },
        components);
}

void EntityCollection::addSimpleSystem(SimpleSystem system) {
    addSimpleSystemWithComponents(system, {});
}

void
EntityCollection::registerComponent(std::string name,
                                    std::function<Component *()> allocator) {
    m_component_types[name] = allocator;
}

std::vector<StateChange> EntityCollection::collectStateChanges() {
    std::vector<StateChange> changes;
    for (auto & entity : m_entities) {
        for (auto & entity_changes : entity.collectStateChanges()) {
            changes.emplace_back(entity.getID(), std::get<0>(entity_changes),
                                 std::get<1>(entity_changes),
                                 std::get<2>(entity_changes));
        }
    }
    return changes;
}

void EntityCollection::handleEntityStateChange(json11::Json entity) {
    if (!entity["id"].is_number()) {
        return;
    }
    if (!entity["component"].is_string()) {
        return;
    }
    if (!entity["field"].is_string()) {
        return;
    }

    unsigned int id = entity["id"].int_value();
    std::string component = entity["component"].string_value();
    std::string field = entity["field"].string_value();
    json11::Json value = entity["value"];
    auto ent_it =
        std::find_if(m_entities.begin(), m_entities.end(),
                     [=](Entity & entity) { return entity.getID() == id; });
    Entity * ent = nullptr;
    if (ent_it == m_entities.end()) {
        ent = &createEntity(id);
    } else {
        ent = &(*ent_it);
    }
    if (!ent->hasComponent(component)) {
        if (m_component_types.count(component) == 0) {
            // We don't know what type of component this is, just give up
            return;
        }
        ent->addComponent(m_component_types[component]());
    }
    (*ent)[component]->setField(field, entity["value"]);
}

void EntityCollection::cycle() {
    // Of course currently this is horribly inefficient. The hope is that
    // systems can specify their required components when they register
    // with the collection so the collection can optimise the interation.
    for (auto & system_config : m_systems) {
        auto system = std::get<0>(system_config);
        auto components = std::get<1>(system_config);
        for (auto & entity : m_entities) {
            bool apply_system = true;
            for (auto & component : components) {
                apply_system = apply_system && entity.hasComponent(component);
            }
            if (apply_system) {
                system(this, entity);
            }
        }
    }
    m_frame++;
}

Entity & EntityCollection::createEntity(unsigned int id) {
    m_entities.emplace_back(id);
    return m_entities.back();
}

Entity & EntityCollection::get(unsigned int id) {
    for (auto & e : m_entities) {
        if (e.getID() == id) {
            return e;
        }
    }
    // What do I do here?
    throw std::runtime_error(
        fmt::format("Entity with ID <{}> does not exist", id));
}

} // namespace entity
