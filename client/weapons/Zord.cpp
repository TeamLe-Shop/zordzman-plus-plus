#include "weapons/Zord.hpp"

#include <iostream>

Zord::Zord() : BaseWeapon("Zord", 0, 7, COMBAT, ZORD) {}

Zord::~Zord() {}

// I want void Zord::use(Player* player)
void Zord::use() {
    std::cout << "hmm yiss\n";
}

