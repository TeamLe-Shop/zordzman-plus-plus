#include "weapons/Zord.hpp"

using namespace client::weapon;
#include <iostream>

Zord::Zord() : BaseWeapon("Zord", 0, 7, COMBAT, ZORD) {}

Zord::~Zord() {}

void Zord::use(Player * player) { std::cout << "hmm yiss\n"; }
