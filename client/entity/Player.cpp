#include "Player.hpp"
#include "gfx/drawingOperations.hpp"
#include "Client.hpp"

namespace client {
using namespace weapon;
using namespace mob;
Player::Player(std::string username, float x, float y, float speed)
    : Mob(x, y, speed), m_username(username) {
    m_health = 100;
}

void Player::render() const {
    using namespace drawingOperations;
    //sys::Texture & tex = Client::get().m_resources.getTexture("main");

    // Depending on their direction, render a different sprite.
    // The sprite will animate based on how many "steps" it has taken.
    /**
    switch (m_direction) {
    case SOUTH:
        drawSpriteFromTexture(tex, 0, 2, m_x, m_y, 32, 32, 16,
                              m_distanceWalked < 30 ? SpriteFlip::None
                                                    : SpriteFlip::Horizontal);
        break;
    case NORTH:
        drawSpriteFromTexture(tex, 3, 2, m_x, m_y, 32, 32, 16,
                              m_distanceWalked < 30 ? SpriteFlip::None
                                                    : SpriteFlip::Horizontal);
        break;
    case WEST:
        drawSpriteFromTexture(tex, m_distanceWalked < 30 ? 1 : 2, 2, m_x, m_y,
                              32, 32, 16, SpriteFlip::Horizontal);
        break;
    case EAST:
        drawSpriteFromTexture(tex, m_distanceWalked < 30 ? 1 : 2, 2, m_x, m_y,
                              32, 32, 16, SpriteFlip::None);
        break;
    }
    */
}

void Player::tick() {
    // Handle input, mainly movement.
    input();

    // Keep the "distance walked" between 60 and 0.
    if (m_distanceWalked > 60)
        m_distanceWalked = 0;
    else if (m_distanceWalked < 0)
        m_distanceWalked = 60;

    if (weapon_delay)
        weapon_delay--;
}

void Player::input() {
    // Get the current keyboard state. This will have information
    // about what keys are pressed n shit
    Uint8 const * keys = SDL_GetKeyboardState(nullptr);

    if (!weapon_delay) {
        if (keys[SDL_SCANCODE_SPACE]) {
            getCurrentWeapon()->use();
        }
    }

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

Player * Player::clone() const { return new Player(*this); }

std::string Player::getUsername() const { return m_username; }

BaseWeapon * Player::getCombatWeapon() { return m_combat_weapon; }

void Player::setCombatWeapon(BaseWeapon * b) { m_combat_weapon = b; }

bool Player::holdingCombatWeapon() { return m_current_weapon == 0; }

BaseWeapon * Player::getSpecialWeapon() { return m_special_weapon; }

void Player::setSpecialWeapon(BaseWeapon * b) { m_special_weapon = b; }

bool Player::holdingSpecialWeapon() { return m_current_weapon == 1; }

BaseWeapon * Player::getCurrentWeapon() {
    return m_current_weapon == 0 ? m_combat_weapon : m_special_weapon;
}
} // namespace client
