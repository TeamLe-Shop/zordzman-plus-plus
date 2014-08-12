#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

#include <iostream>
#include <SDL.h>

float steps = 0;

Player::Player(float x, float y, float speed) : Mob(x, y, speed) {
    m_health = 100;
}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    getSheet("main").setSpriteSize(16);
    switch (m_direction) {
    case SOUTH:
        drawSpriteFromSheet(getSheet("main"), 0, 2, m_x, m_y, 32, 32,
                            steps < 30 ? NO_FLIP : X_FLIP);
        break;
    case NORTH:
        drawSpriteFromSheet(getSheet("main"), 3, 2, m_x, m_y, 32, 32,
                            steps < 30 ? NO_FLIP : X_FLIP);
        break;
    case WEST:
        drawSpriteFromSheet(getSheet("main"), steps < 30 ? 1 : 2, 2, m_x, m_y,
                            32, 32, X_FLIP);
        break;
    case EAST:
        drawSpriteFromSheet(getSheet("main"), steps < 30 ? 1 : 2, 2, m_x, m_y,
                            32, 32, NO_FLIP);
        break;
    }
    getSheet("main").setSpriteSize(8);
}

void Player::tick() {
    input();

    if (steps > 60)
        steps = 0;
    else if (steps < 0)
        steps = 60;
}

void Player::input() {
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
}

void Player::moveUp() {
    steps -= m_speed * 0.8;
    m_y -= m_speed;
    m_direction = NORTH;
}

void Player::moveDown() {
    steps += m_speed * 0.8;
    m_y += m_speed;
    m_direction = SOUTH;
}

void Player::moveLeft() {
    steps -= m_speed * 0.8;
    m_x -= m_speed;
    m_direction = WEST;
}

void Player::moveRight() {
    steps += m_speed * 0.8;
    m_x += m_speed;
    m_direction = EAST;
}

Player *Player::clone() const { return new Player(*this); }
