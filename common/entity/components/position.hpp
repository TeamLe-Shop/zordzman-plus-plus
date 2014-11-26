#pragma once

#include <string>

#include "common/entity/component.hpp"

namespace entity {

class PositionComponent : public Component {

public:
    PositionComponent()
        : m_x(this, 0.0),
          m_y(this, 0.0) {};

    PositionComponent(double x, double y)
        : m_x(this, x),
          m_y(this, y) {};

    virtual ~PositionComponent() {}

    static std::string getComponentName();
    std::string getName();

    Stateless<double> m_x;
    Stateless<double> m_y;
};


}  // namespace entity
