#pragma once
#include "gfx/SpriteSheet.hpp"

static SpriteSheet mainSheet("resources/spritesheet.png", 8);

namespace globalResources {

SpriteSheet getMainSheet();

}
