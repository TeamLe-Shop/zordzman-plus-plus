#pragma once

namespace client {
/// @brief The namespace containing all weapons.
namespace weapon {

/// @brief The different Weapon slots.
///
/// BOTH is only used by BlankWeapon.
enum WeaponSlot { COMBAT, SPECIAL, BOTH };

/// @brief The different types of weapons.
enum WeaponType { BLANK, ZORD };

} // namespace weapon
} // namespace client
