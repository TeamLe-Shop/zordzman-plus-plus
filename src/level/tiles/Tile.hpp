#pragma once

typedef unsigned char byte;

namespace tile {

const byte GRASS = 0,
           FLOWER = 1,
           WATER  = 2;

byte render(byte a, int tick);
}
