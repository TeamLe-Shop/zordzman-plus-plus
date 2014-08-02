#include "SpriteSheet.hpp"

unsigned int SpriteSheet::getTileSize() const { return m_tile_size; }

SpriteSheet::SpriteSheet(const std::string &filename) : Texture(filename) {}
