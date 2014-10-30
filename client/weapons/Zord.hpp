#pragma once

#include "weapons/BaseWeapon.hpp"

namespace client {
class Player;
namespace weapon {

/// @brief The main weapon of the gayme.

class Zord : public BaseWeapon {
public:
    Zord();
    ~Zord();

    void use_with_player(Player * player);
};
} // namespace weapon
} // namespace client
