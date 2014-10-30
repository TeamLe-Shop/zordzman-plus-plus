#include "weapons/Zord.hpp"

#include <iostream>

namespace client {
namespace weapon {
Zord::Zord() : BaseWeapon("Zord", 0, 7, COMBAT, ZORD) {}

void Zord::use_with_player(Player *) { std::cout << "hmm yiss\n"; }
} // namespace weapon
} // namespace client
