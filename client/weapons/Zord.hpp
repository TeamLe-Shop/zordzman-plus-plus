#pragma once

#include "weapons/BaseWeapon.hpp"

class Player;

/// @brief The main weapon of the gayme.

class Zord : public client::weapon::BaseWeapon {
public:
    Zord();
    ~Zord();

    void use(Player * player);
};
