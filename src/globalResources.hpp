#pragma once
#include "gfx/SpriteSheet.hpp"

namespace globalResources {

void init();
SpriteSheet const &getSheet(char const *const key);
void free();
}
