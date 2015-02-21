#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include "common/extlib/cppformat/format.h"
#include "common/extlib/json11/json11.hpp"

namespace entity {

// Field name, value
typedef std::tuple<std::string, json11::Json> ComponentStateChange;

/// Baseclass for implementing entity components.
///
/// Components are groupings of 'fields' which hold the state of an entity
/// for which their containing component belongs to.
///
/// There are two types of fields, stateful and stateless. The difference is
/// how changes in their value are propagated to clients. Stateful fields are
/// sent over the TCP connection as an `entity.state` message which names the
/// changed entity component field as well as the new value for that field.
/// Because of this stateful fields should be reserved for values that change
/// infrequently but the change of value must be propagated reliably.
///
/// Alternately, stateless fields are sent via UDP. When the entities state is
/// periodically sent to clients the stateless fields are serialised and packed
/// into a UDP packet.
///
/// Each component subclass must have a unique string ID. This is provided
/// by the @ref getName method but its also advisable to have a static method
/// which returns the same value.
class Component {
    using Setter = std::function<void(json11::Json value)>;

public:

    virtual ~Component() {}

    /// Get the unique component ID.
    ///
    /// This should return the same value for all instances of the component.
    virtual std::string getName() = 0;

    bool isDirty() { return m_is_dirty; }
    void markDirty() { m_is_dirty = true; }

    /// Set a stateful field from an arbitrary JSON value.
    ///
    /// This calls the setter for the given `field_name` that was previously
    /// registered @ref addStateSetter, passing the JSON value in as the
    /// setter argument.
    ///
    /// If there was no setter registered for the `field_name` then this method
    /// does nothing.
    void setField(std::string field_name, json11::Json value) {
        if (m_setters.count(field_name) == 1) {
            m_setters[field_name](value);
        }
        // Fail silently?
    }

    /// Register a function to be called for setting stateful field values.
    ///
    /// The registered function (the *setter*) should take a single json11:Json
    /// argument which represents the new value for a field. When @ref setField
    /// is called with the same `field_name` the setter will be called.
    ///
    /// Strictly speaking this should never be invoked '*manually*'.
    /// @ref Stateful fields will register their own setters when they're
    /// initialised.
    void addStateSetter(std::string field_name, Setter setter) {
        m_setters[field_name] = setter;
    }

    void markStateChange(std::string field_name, json11::Json value) {
        m_state_changes.emplace_back(field_name, value);
    }

    std::vector<ComponentStateChange> collectStateChanges() {
        std::vector<ComponentStateChange> copy = m_state_changes;
        m_state_changes.clear();
        return copy;
    }

private:
    std::vector<ComponentStateChange> m_state_changes;
    bool m_is_dirty;
    std::map<std::string, Setter> m_setters;
};

template <class T> class Field {
public:
    Field(Component * component, T initial) {
        m_component = component;
        m_value = initial;
    }

    virtual ~Field() {}

    void set(T value) {
        T old_value = m_value;
        m_value = value;
        onStateChange(old_value, value);
    }

    T get() { return m_value; }

private:
    virtual void onStateChange(T old, T new_) = 0;

protected:
    T m_value;
    Component * m_component;
};

/// Utility class for converting JSON objects to their native equivalents.
///
/// This implements a number of conversion operators which converts to
/// json11::Json objects to C++-native objects.
class JSONFieldValue {
public:
    JSONFieldValue(json11::Json value) : m_value(value) {}

    operator int() { return m_value.int_value(); };
    operator std::string() { return m_value.string_value(); };

private:
    json11::Json m_value;
};

template <class T> class Stateful : public Field<T> {
    using Field<T>::m_component;
    using Field<T>::m_value;

public:
    Stateful(Component * component, std::string name, T initial)
        : Field<T>::Field(component, initial) {
        m_component->addStateSetter(
            name,
            std::bind(&Stateful<T>::setFromJSON, this, std::placeholders::_1));
        m_component->markStateChange(name, initial);
        m_name = name;
    }

private:
    std::string m_name;

    void setFromJSON(json11::Json value) {
        m_value = static_cast<T>(JSONFieldValue(value));
    }

    void onStateChange(T /*old*/, T new_) {
        m_component->markStateChange(m_name, new_);
    }
};

template <class T> class Stateless : public Field<T> {
    using Field<T>::m_component;

public:
    Stateless(Component * component, T initial)
        : Field<T>::Field(component, initial) {}

private:
    void onStateChange(T /*old*/, T /*new_*/) { m_component->markDirty(); }
};

// Component
// * May have zero or more stateful fields
// * May have zero or more stateless fields
// * Should be uniquely identifiable among other components

// Stateful Transition Field
// * Must be named for identification in messages
// * The data must be serialisable as json11::Json
// * Must communicate their change of state to the entity collection (e.g. via
// component)

// Stateless Transition Field
// * The parent component must know how to serialise all its statelss fields
// * The parent component must know how to deserialise all its stateless fields
// * The fields must be able to communicate its state change to the entity
// collection (e.g. via component)

} // namespace entity
