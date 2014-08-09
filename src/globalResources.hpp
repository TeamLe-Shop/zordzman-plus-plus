#pragma once
#include "gfx/SpriteSheet.hpp"

namespace globalResources {

void init();
SpriteSheet &getSheet(char const *const key);
void free();
}
