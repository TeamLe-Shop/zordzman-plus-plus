#pragma once

#include <string>

#include "common/entity/component.hpp"

namespace entity {

class PositionComponent : public Component {
public:
    PositionComponent() : m_x(this, m_name_x, 0.0), m_y(this, m_name_y, 0.0){};

    PositionComponent(double x, double y) : m_x(this, m_name_x, x), m_y(this,
                                                m_name_y, y){};

    virtual ~PositionComponent() {}

    static std::string getComponentName();
    std::string getName();

    static Component * new_();

    Stateful<double> m_x;
    Stateful<double> m_y;

    static const std::string m_name_x;
    static const std::string m_name_y;
};

} // namespace entity
