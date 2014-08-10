#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

int steps = 0;

Player::Player(float nx, float ny) {
    x = nx;
    y = ny;
}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    getSheet("main").setSpriteSize(16);
    drawSpriteFromSheet(getSheet("main"), 0, 2, x, y, 32, 32, steps %
                        60 < 30 ? NO_FLIP : X_FLIP);
    getSheet("main").setSpriteSize(8);
}

void Player::tick() {
    
}

void Player::moveUp() { steps++; y -= speed; }

void Player::moveDown() { steps++; y += speed; }

void Player::moveLeft() { steps++; x -= speed; }

void Player::moveRight() { steps++; x += speed; }
