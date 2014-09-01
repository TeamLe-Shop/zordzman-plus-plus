#pragma once

#include "weapons/BaseWeapon.hpp"

/// @brief The main weapon of the gayme.
class Zord : public client::weapon::BaseWeapon {
public:
    Zord();

    void use();
};
