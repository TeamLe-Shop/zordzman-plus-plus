#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "Game.hpp"

#include <SDL.h>

Player::Player(std::string username, float x, float y, float speed)
    : Mob(x, y, speed), m_username(username) {
    m_health = 100;
}

void Player::render() const {
    using namespace drawingOperations;
    SpriteSheet &sheet = Game::get().resources.getSheet("main");

    // Set the spritesize to 16 x 16 for now.
    sheet.setSpriteSize(16);
    // Depending on their direction, render a different sprite.
    // The sprite will animate based on how many "steps" it has taken.
    switch (m_direction) {
    case SOUTH:
        drawSpriteFromSheet(sheet, 0, 2, m_x, m_y, 32, 32,
                            m_distanceWalked < 30 ? NO_FLIP : X_FLIP);
        break;
    case NORTH:
        drawSpriteFromSheet(sheet, 3, 2, m_x, m_y, 32, 32,
                            m_distanceWalked < 30 ? NO_FLIP : X_FLIP);
        break;
    case WEST:
        drawSpriteFromSheet(sheet, m_distanceWalked < 30 ? 1 : 2, 2, m_x, m_y,
                            32, 32, X_FLIP);
        break;
    case EAST:
        drawSpriteFromSheet(sheet, m_distanceWalked < 30 ? 1 : 2, 2, m_x, m_y,
                            32, 32, NO_FLIP);
        break;
    }
    // Set the sprite size back to 8x8.
    sheet.setSpriteSize(8);

    float username_x = (m_x + 16) - m_username.size() * 4;
    float username_y = m_y - 12;
    float username_width = m_username.size() * 8;
    glColor4f(0.2, 0.2, 0.2, 0.2);
    drawRectangle(username_x, username_y, username_width, 8);
    glColor4f(1, 1, 1, 1);
    drawText(m_username, username_x, username_y, 8, 8);
}

void Player::tick() {
    // Handle input, mainly movement.
    input();

    // Keep the "distance walked" between 60 and 0.
    if (m_distanceWalked > 60)
        m_distanceWalked = 0;
    else if (m_distanceWalked < 0)
        m_distanceWalked = 60;
}

void Player::input() {
    // Get the current keyboard state. This will have information
    // about what keys are pressed n shit
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
    // and their distance_walked by (speed * 0.8), as well as
    // change their direction to NORTH.
    m_distanceWalked -= m_speed * 0.8;
    m_y -= m_speed;
    m_direction = NORTH;
}

void Player::moveDown() {
    // Increase the player's Y position by their speed
    // and their distance_walked by (speed * 0.8), as well as
    // change their direction to SOUTH.
    m_distanceWalked += m_speed * 0.8;
    m_y += m_speed;
    m_direction = SOUTH;
}

void Player::moveLeft() {
    // Decrease the player's Z position by their speed
    // and their distance_walked by (speed * 0.8), as well as
    // change their direction to WEST.
    m_distanceWalked -= m_speed * 0.8;
    m_x -= m_speed;
    m_direction = WEST;
}

void Player::moveRight() {
    // Increase the player's X position by their speed
    // and their distance_walked by (speed * 0.8), as well as
    // change their direction to EAST.
    m_distanceWalked += m_speed * 0.8;
    m_x += m_speed;
    m_direction = EAST;
}

Player *Player::clone() const { return new Player(*this); }

std::string Player::getUsername() const { return m_username; }

BaseWeapon Player::getCombatWeapon() { return m_combat_weapon; }

bool Player::holdingCombatWeapon() {
    return m_current_weapon == &m_combat_weapon;
}

BaseWeapon Player::getSpecialWeapon() { return m_special_weapon; }

bool Player::holdingSpecialWeapon() {
    return m_current_weapon == &m_special_weapon;
}

BaseWeapon Player::getCurrentWeapon() { return *m_current_weapon; }
