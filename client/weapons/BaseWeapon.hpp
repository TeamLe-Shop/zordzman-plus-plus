#pragma once

#include "weapons/weapon.hpp"
#include <string>

namespace client {

namespace weapon {

/// @brief The base weapon for every weapon in the game
class BaseWeapon {
public:
    /// @brief Constructor for BaseWeapon.
    ///
    /// This should only be called in another weapon's constructor.
    ///
    /// @param name   The name of the weapon
    /// @param x_tile The x position of the tile on the spritesheet
    ///               the weapon icon should be rendered from.
    /// @param y_tile The y position of the tile on the spritesheet
    ///               the weapon icon should be rendered from.
    /// @param slot   The weapon slot.
    /// @param type   The type of weapon.
    BaseWeapon(std::string name, int x_tile, int y_tile, WeaponSlot slot,
               WeaponType type);

    /// @brief Get the name of this weapon
    std::string getName();
    /// @brief Get the slot this weapon goes into.
    WeaponSlot getSlot();
    /// @brief Get the type of weapon this is.
    WeaponType getType();
    /// @brief Called when the player uses the item.
    virtual void use();
    virtual ~BaseWeapon();

public:
    const int x_tile, y_tile;

private:
    std::string m_name;
    WeaponSlot m_slot;
    WeaponType m_type;
};

} // namespace weapon
} // namespace client
