#pragma once

namespace client {
/// The namespace containing all weapons.
namespace weapon {

/// The different Weapon slots.
///
/// BOTH is only used by BlankWeapon.
enum WeaponSlot { COMBAT, SPECIAL, BOTH };

/// The different types of weapons.
enum WeaponType { BLANK, ZORD };

} // namespace weapon
} // namespace client
