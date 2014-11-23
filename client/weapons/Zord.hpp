#pragma once

#include "weapons/BaseWeapon.hpp"

namespace client {
class Player;
namespace weapon {

/// The main weapon of the gayme.

class Zord : public BaseWeapon {
public:
    Zord();

    /// Function called when the Player uses this item.
    void use_with_player(Player * player);
};
} // namespace weapon
} // namespace client
