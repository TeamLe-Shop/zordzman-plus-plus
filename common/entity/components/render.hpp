#pragma once

#include <string>

#include "common/entity/component.hpp"

namespace entity {

class RenderComponent : public Component {
public:
    RenderComponent(std::string sprite, float alpha) :
        m_sprite(this, m_name_sprite, sprite),
        m_alpha(this, m_name_alpha, alpha) {}

    virtual ~RenderComponent() {}

    static std::string getComponentName();
    std::string getName();

    static Component * new_();

    Stateful<std::string> m_sprite;
    Stateful<float> m_alpha;

    static const std::string m_name_sprite;
    static const std::string m_name_alpha;
};

} // namespace entity
