#pragma once

#include "entity/Player.hpp"
#include "weapons/BaseWeapon.hpp"

/// @brief The main weapon of the gayme.
class Zord : public client::weapon::BaseWeapon {
public:
    Zord();
    ~Zord();
    // I want void use(Player* player)
    void use();
};
