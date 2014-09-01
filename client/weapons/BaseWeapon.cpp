#include "weapons/BaseWeapon.hpp"
#include "weapons/weapon.hpp"

namespace client {
namespace weapon {

BaseWeapon::BaseWeapon(std::string name, int xtile, int ytile, WeaponSlot slot,
                       WeaponType type)
    : x_tile(xtile), y_tile(ytile), m_name(name), m_slot(slot), m_type(type) {}

std::string BaseWeapon::getName() { return m_name; }

WeaponSlot BaseWeapon::getSlot() { return m_slot; }

WeaponType BaseWeapon::getType() { return m_type; }

void BaseWeapon::use() {}

BaseWeapon::~BaseWeapon() {}
}
}
