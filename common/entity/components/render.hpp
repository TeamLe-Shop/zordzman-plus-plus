#pragma once

#include <string>

#include "common/entity/component.hpp"

namespace entity {

class RenderComponent : public Component {
public:
    RenderComponent(std::string sprite, float alpha, double width,
                    double height) :
        m_sprite(this, m_name_sprite, sprite),
        m_alpha(this, m_name_alpha, alpha),
        m_rotation(this, m_name_rotation, 0),
        m_width(this, m_name_width, width),
        m_height(this, m_name_height, height) {}

    virtual ~RenderComponent() {}

    static std::string getComponentName();
    std::string getName();

    static Component * new_();

    Stateful<std::string> m_sprite;
    Stateful<double> m_alpha;
    Stateful<double> m_rotation;
    Stateful<double> m_width;
    Stateful<double> m_height;

    static const std::string m_name_sprite;
    static const std::string m_name_alpha;
    static const std::string m_name_rotation;
    static const std::string m_name_width;
    static const std::string m_name_height;
};

} // namespace entity
