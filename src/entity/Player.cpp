#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

Player::Player(float nx, float ny) {
    x = nx;
    y = ny;
}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    glBegin(GL_QUADS);
    getSheet("main").setSpriteSize(16);
    drawSpriteFromSheet(getSheet("main"), 0, 2, x, y, 32, 32);
    getSheet("main").setSpriteSize(8);
	glEnd();
}