#pragma once

#include "weapons/BaseWeapon.hpp"

namespace client {
class Player;
namespace weapon {

/// @brief The main weapon of the gayme.

class Zord : public BaseWeapon {
public:
    Zord();

    /// @brief Function called when the Player uses this item.
    void use_with_player(Player * player);
};
} // namespace weapon
} // namespace client
