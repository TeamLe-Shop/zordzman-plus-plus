#pragma once

#include "Texture.hpp" // The original Zordzman used a custom SpriteSheet class.
                       // We'll probably enhance Texture.hpp and use that as
                       // a spritesheet.

// TODO: Add more shit

namespace draw {
void sprite(Texture const &spritesheet, int tx, int ty, float x, float y,
            float w, float h);
void rect(float x, float y, float w, float h, bool filled = true);
void line(float x1, float y1, float x2, float y2);
}
