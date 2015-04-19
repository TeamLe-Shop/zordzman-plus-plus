#include "common/entity/components/render.hpp"

namespace entity {

const std::string RenderComponent::m_name_sprite = "sprite";
const std::string RenderComponent::m_name_alpha = "alpha";

std::string RenderComponent::getComponentName() { return "render"; }

std::string RenderComponent::getName() { return "render"; }

Component * RenderComponent::new_() { return new RenderComponent("", 1); }

} // namespace entity
