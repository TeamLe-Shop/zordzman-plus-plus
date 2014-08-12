#pragma once
#include "gfx/SpriteSheet.hpp"

namespace globalResources {

/// @brief Initialize the resources.
void init();
/// @brief Get a spritesheet by its name.
SpriteSheet &getSheet(char const *const key);
/// @brief Free resources
void free();
}
