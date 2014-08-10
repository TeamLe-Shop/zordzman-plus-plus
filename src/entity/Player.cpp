#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

#include <SDL.h>

float steps = 0;

Player::Player(float nx, float ny) {
    x = nx;
    y = ny;
}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    getSheet("main").setSpriteSize(16);
    drawSpriteFromSheet(getSheet("main"), 0, 2, x, y, 32, 32,
                        steps < 30 ? NO_FLIP : X_FLIP);
    getSheet("main").setSpriteSize(8);
}

void Player::tick() {

    Uint8 const *keys = SDL_GetKeyboardState(nullptr);

    if (keys[SDL_SCANCODE_LEFT]) {
        moveLeft();
    } else if (keys[SDL_SCANCODE_RIGHT]) {
        moveRight();
    } else if (keys[SDL_SCANCODE_UP]) {
        moveUp();
    } else if (keys[SDL_SCANCODE_DOWN]) {
        moveDown();
    }

    if (steps > 60)
        steps = 0;
}

void Player::moveUp() {
    steps += speed * 0.8;
    y -= speed;
}

void Player::moveDown() {
    steps += speed * 0.8;
    y += speed;
}

void Player::moveLeft() {
    steps += speed * 0.8;
    ;
    x -= speed;
}

void Player::moveRight() {
    steps += speed * 0.8;
    x += speed;
}

Player *Player::clone() const { return new Player(*this); }
