#pragma once

namespace client {
typedef unsigned char byte;

namespace tile {
const byte GRASS = 0, FLOWER = 1, WATER = 2;

/// This returns the spritesheet co-ordinates of what tile to render
///        based on `id`, the ID of the tile, and `tick`, the animation tick.
///
/// @param id The id of the tile to draw.
/// @param tick The current animation tick.
byte render(byte id, int tick);
} // namespace tile
} // namespace client
