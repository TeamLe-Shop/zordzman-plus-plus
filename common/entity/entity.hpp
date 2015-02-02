#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "common/extlib/json11/json11.hpp"

#include "common/entity/component.hpp"

#define COMPONENT(ent, type, as)                                               \
    auto as = static_cast<type *>(ent[type::getComponentName()])

/// This namespace provides the means for representing game state using the
/// entity-component-system (ECS) model.
///
/// As implied by the name, this model defines three core concepts:
///
/// Entities
/// --------
/// Entities are dumb collections of data. Related data is grouped together
/// into components. Each entity is uniquely identifiable throughout the game's
/// existence via a numeric ID.
///
/// Components
/// ----------
/// Components are the logical grouping of entity data fields. Components are
/// owned by entities, and each component represents a 'trait' exhibited by the
/// parent entity. For example, there may be an component type for holding
/// positional data (a.k.a. X, Y coordinates). An entity with a position
/// component can be said to exhibit the 'positional' trait.
///
/// Each component type should be uniquely identifiable among all over
/// component types by a string ID.
///
/// Systems
/// -------
/// Systems are routines that act on these traits. They are responsible for
/// implementing the 'game logic'. Each system is called once for each
/// simulation cycle.
///
/// Systems may register an interest in entities that only possess certain
/// components. For example, a trivial system may wish to only be applied to
/// entities with the aforementioned position component. This system then
/// might then increment the coordinates each time it is invoked.
namespace entity {

// Entity ID, component name, field name, new value as JSON
typedef std::tuple<unsigned int, std::string, std::string, json11::Json>
    StateChange;
typedef std::tuple<std::string, std::string, json11::Json> EntityStateChange;

/// A game entity.
///
/// Entities are just collections of components. Each component is uniquely
/// identified by a numberic ID. The ID is assigned by the creator of the
/// entity, therefore `Entity` instances should never be created manually.
/// Instead they should be created via `EntityCollection`s.
class Entity {
public:
    Entity(unsigned int id);

    /// Retrieve the unique ID for the entity.
    unsigned int getID() const;

    /// Add a new component to the entity.
    ///
    /// The entity will be responsible for freeing the component when the
    /// entity is destroyed.
    ///
    /// If a component of the same type already exists for the entity it will
    /// be replaced with the new one. The old component will be freed.
    void addComponent(Component * component);

    /// Check if a component exists on the entity by component ID.
    bool hasComponent(std::string name);

    /// This will collect stateful field changes from all components attached
    /// to the entity.
    std::vector<EntityStateChange> collectStateChanges();

    /// Access a component by component ID.
    Component * operator[](std::string name);

private:
    unsigned int m_id;
    std::map<std::string, std::unique_ptr<Component>> m_components;
};

/// Game entity manager.
///
/// Instances of this class are responsible for managing all entities,
/// running the simulation and sharing state between the server and clients.
class EntityCollection {
    using System = std::function<void(EntityCollection *, Entity &)>;
    using SimpleSystem = std::function<void(Entity &)>;

public:
    EntityCollection();

    /// Get the current simulation frame ID.
    unsigned int getFrame();

    /// Create a new Entity to be managed by the collection.
    ///
    /// The collection will allocate a unique ID for the newly created entity.
    Entity & createEntity();

    /// Remove an entity by it's id.
    void removeEntity(unsigned int id);

    /// Register a 'system' callable to the collection.
    ///
    /// Systems should be callables that expect two arguments: a pointer to the
    /// collection the system is registered with and a reference to the entity
    /// the system is being applied to. Systems should return void.
    ///
    /// When the collection is simulated via `cycle` all systems are applied to
    /// the entities within the collection. When registering a system with the
    /// collection a list of component IDs may be given to filter the entities
    /// that the system is applied to during simulation.
    ///
    /// Only entities that possess all the given component types will have the
    /// system applied to them. However, if no component IDs are given when
    /// registering a system then the system will be applied to all entities,
    /// regardless of the components they possess.
    void addSystem(System system,
                   std::initializer_list<std::string> components);

    /// Register a system for all components.
    ///
    /// The same as @ref addSystem with the second parameter being an empty
    /// list.
    void addSystem(System system);

    /// Register a 'simple' system.
    ///
    /// The same as @ref addSystem except that the signature for the system
    /// callable slightly different. Simple systems do not take a pointer to
    /// the collection as their first argument. They should still accept the
    /// Entity reference as their sole argument and return void.
    void addSystem(SimpleSystem system,
                   std::initializer_list<std::string> components);

    /// Register a simple system for all components.
    void addSystem(SimpleSystem system);
    void registerComponent(std::string name,
                           std::function<Component *()> allocator);
    std::vector<StateChange> collectStateChanges();

    /// Handler for `entity.state` messages.
    ///
    /// This is a method suitable for attaching to a net::MessageProcessor as
    /// a message handler.
    ///
    /// This method handles `entity.state` messages which are sent by a server
    /// in order to indicate a entity change of state. Each message has the
    /// following fields:
    ///
    ///  * `entity` -- the ID of the entity as number.
    ///  * `component` -- the affected component ID as a string.
    ///  * `field` -- the name of the field whose value change as string.
    ///  * `value` -- the new value of the field as any JSON type.
    ///
    /// @code{.json}
    /// {
    ///     "entity": 123,
    ///     "component": "character",
    ///     "field": "name",
    ///     "value": "The Real Slim Shady"
    /// }
    /// @endcode
    ///
    /// If any of the fields don't have the correct type then the message is
    /// ignored.
    ///
    /// If an entity with the recieved ID doesn't exist within the collection
    /// then it will be created. Additionally, if the named component doesn't
    /// exist on the entity it will be created. Note that the component must
    /// be registered with the collection via registerComponent. If no
    /// component with the given name is registered for the collection then the
    /// message is ignored.
    ///
    /// Component::setField will be called on the named component in order to
    /// set the new field value.
    void handleEntityStateChange(json11::Json entity);

    /// Execute a single simulation step.
    ///
    /// This executes all the systems that are registered on the collection
    /// on the entities applicable to each system. Systems are executed in the
    /// order they were added to the collection.
    ///
    /// This increments the frame ID counter by one.
    void cycle();

private:
    unsigned int m_entity_id;
    unsigned int m_frame;
    std::vector<Entity> m_entities;
    std::vector<std::tuple<System, std::vector<std::string>>> m_systems;
    std::map<std::string, std::function<Component *()>> m_component_types;

    Entity & createEntity(unsigned int id);
};

} // namespace entity
