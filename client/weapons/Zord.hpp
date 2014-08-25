#pragma once

#include "weapons/BaseWeapon.hpp"
using namespace client::weapon;

/// @brief The main weapon of the gayme.
class Zord : public BaseWeapon {
public:
    Zord();

    void use();
};