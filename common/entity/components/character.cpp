#include "common/entity/components/character.hpp"

namespace entity {

const std::string CharacterComponent::m_name_name = "name";
const std::string CharacterComponent::m_name_health = "health";
const std::string CharacterComponent::m_name_max_health = "max_health";

std::string CharacterComponent::getComponentName() {
    return "character";
}

std::string CharacterComponent::getName() {
    return "character";
}

}  // namespace entity
