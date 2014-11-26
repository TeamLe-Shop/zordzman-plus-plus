#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "common/extlib/cppformat/format.h"
#include "common/extlib/json11/json11.hpp"


namespace entity {

class Component {

public:
    virtual ~Component() {}
    virtual std::string getName() = 0;

    bool isDirty() { return m_is_dirty; }
    void markDirty() { m_is_dirty = true; }
    void markStateChange(std::string field_name, json11::Json value) {
        m_state_changes.emplace_back(field_name, value);
    }

private:
    std::vector<std::tuple<std::string, json11::Json>> m_state_changes;
    bool m_is_dirty;
};


template <class T>
class Field {

public:
    Field(Component *component, T initial) {
        m_component = component;
        m_value = initial;
    }

    virtual ~Field() {}

    void set(T value) {
        T old_value = m_value;
        m_value = value;
        onStateChange(old_value, value);
    }

    T get() {
        return m_value;
    }

private:
    virtual void onStateChange(T old, T new_) = 0;

protected:
    T m_value;
    Component *m_component;
};


template <class T>
class Stateful : public Field<T> {

using Field<T>::m_component;

public:
    Stateful(Component *component, std::string name, T initial)
        : Field<T>::Field(component, initial) {
        m_name = name;
    }

private:
    std::string m_name;

    void onStateChange(T old, T new_) {
        fmt::print("Stateful: {} -> {}\n", old, new_);
        m_component->markStateChange(m_name, new_);

    }

};


template <class T>
class Stateless : public Field<T> {

using Field<T>::m_component;

public :
    Stateless(Component *component, T initial)
        : Field<T>::Field(component, initial) {}

private:
    void onStateChange(T old, T new_) {
        fmt::print("Stateless: {} -> {}\n", old, new_);
        m_component->markDirty();
    }
};


// Component
// * May have zero or more stateful fields
// * May have zero or more stateless fields
// * Should be uniquely identifiable among other components


// Stateful Transition Field
// * Must be named for identification in messages
// * The data must be serialisable as json11::Json
// * Must communicate their change of state to the entity collection (e.g. via component)


// Stateless Transition Field
// * The parent component must know how to serialise all its statelss fields
// * The parent component must know how to deserialise all its stateless fields
// * The fields must be able to communicate its state change to the entity collection (e.g. via component)


}  // namespace entity
