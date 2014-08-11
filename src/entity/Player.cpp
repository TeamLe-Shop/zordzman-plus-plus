#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

#include <SDL.h>

float steps = 0;

Player::Player(float x, float y, float speed) : Entity(x, y), m_speed(speed) {}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    getSheet("main").setSpriteSize(16);
    drawSpriteFromSheet(getSheet("main"), 0, 2, m_x, m_y, 32, 32,
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
    steps += m_speed * 0.8;
    m_y -= m_speed;
}

void Player::moveDown() {
    steps += m_speed * 0.8;
    m_y += m_speed;
}

void Player::moveLeft() {
    steps += m_speed * 0.8;
    ;
    m_x -= m_speed;
}

void Player::moveRight() {
    steps += m_speed * 0.8;
    m_x += m_speed;
}

Player *Player::clone() const { return new Player(*this); }
