#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "globalResources.hpp"

#include <iostream>
#include <SDL.h>

float steps = 0;

Player::Player(float x, float y, float speed) : Mob(x, y, speed) {
    // Set the initial health to 100.
    m_health = 100;
}

void Player::render() {
    using namespace drawingOperations;
    using namespace globalResources;

    // Set the spritesize to 16 x 16 for now.
    getSheet("main").setSpriteSize(16);
    // Depending on their direction, render a different sprite.
    // The sprite will animate based on how many "steps" it has taken.
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
    // Set the sprite size back to 8x8.
    getSheet("main").setSpriteSize(8);
}

void Player::tick() {
    // Handle input, mainly movement.
    input();

    if (steps > 60)
        steps = 0;
    else if (steps < 0)
        steps = 60;
}

void Player::input() {
    // Get the current keyboard state
    Uint8 const *keys = SDL_GetKeyboardState(nullptr);

    // Check if they've pressed the arrow keys, and move them if they have.
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
    // Decrease the player's Y position by their speed
    // and their steps by (speed * 0.8), as well as
    // change their direction to NORTH.
    steps -= m_speed * 0.8;
    m_y -= m_speed;
    m_direction = NORTH;
}

void Player::moveDown() {
    // Increase the player's Y position by their speed
    // and their steps by (speed * 0.8), as well as
    // change their direction to SOUTH.
    steps += m_speed * 0.8;
    m_y += m_speed;
    m_direction = SOUTH;
}

void Player::moveLeft() {
    // Decrease the player's Z position by their speed
    // and their steps by (speed * 0.8), as well as
    // change their direction to WEST.
    steps -= m_speed * 0.8;
    m_x -= m_speed;
    m_direction = WEST;
}

void Player::moveRight() {
    // Increase the player's X position by their speed
    // and their steps by (speed * 0.8), as well as
    // change their direction to EAST.
    steps += m_speed * 0.8;
    m_x += m_speed;
    m_direction = EAST;
}

Player *Player::clone() const { return new Player(*this); }
