#pragma once

#include <string>

#include "common/entity/component.hpp"


namespace entity {

class CharacterComponent : public Component {

public:
    CharacterComponent()
        : m_name(this, m_name_name, ""),
          m_health(this, m_name_health, 0),
          m_max_health(this, m_name_max_health, 0),
          m_alive(this, true) {}
    CharacterComponent(std::string name)
        : m_name(this, m_name_name, name),
          m_health(this, m_name_health, 0),
          m_max_health(this, m_name_max_health, 0),
          m_alive(this, true) {}
    CharacterComponent(std::string name, int health)
        : m_name(this, m_name_name, name),
          m_health(this, m_name_health, health),
          m_max_health(this, m_name_max_health, health),
          m_alive(this, true) {}

    virtual ~CharacterComponent() {}

    static std::string getComponentName();
    std::string getName();

    Stateful<std::string> m_name;
    Stateful<int> m_health;
    Stateful<int> m_max_health;
    Stateless<bool> m_alive;

protected:
    static const std::string m_name_name;
    static const std::string m_name_health;
    static const std::string m_name_max_health;
};

}  // namespace entity
