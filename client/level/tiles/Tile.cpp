#include "Tile.hpp"

using namespace tile;

namespace tile {

byte render(byte a, int ticks) {
    int animticks = ticks % 180;
    switch (a) {
    case GRASS:
        return 0;
    case FLOWER:
        return 1;
    case WATER:
        if (animticks >= 60 && animticks < 120) {
            return 3;
        } else if (animticks >= 120 && animticks < 180) {
            return 4;
        }
        return 2;
    }
    return 0;
}

} // namespace tile
