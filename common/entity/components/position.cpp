#include "common/entity/components/position.hpp"

namespace entity {

const std::string PositionComponent::m_name_x = "x";
const std::string PositionComponent::m_name_y = "y";

std::string PositionComponent::getComponentName() { return "position"; }

std::string PositionComponent::getName() { return "position"; }

Component * PositionComponent::new_() { return new PositionComponent(); }

} // namespace entity
