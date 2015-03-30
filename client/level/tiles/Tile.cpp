#include "Tile.hpp"

namespace client {
namespace tile {

std::string render(byte id, int ticks) {
    int animticks = ticks % 240;
    switch (id) {
    case GRASS:
        return "grass";
    case FLOWER:
        return "flower";
    case WATER:
        if (animticks >= 60 && animticks < 120) {
            return "water_1";
        } else if (animticks >= 120 && animticks < 180) {
            return "water_2";
        } else if (animticks >= 180 && animticks < 240) {
            return "water_1";
        }
        return "water";
    }
    return "grass";
}

} // namespace tile
} // namespace client
