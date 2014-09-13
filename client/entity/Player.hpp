#pragma once

#include "Entity.hpp"
#include "Mob.hpp"
#include "weapons/weapon.hpp"
#include "weapons/weaponList.hpp"

namespace client {
using namespace mob;
using namespace weaponList;
using namespace weapon;

class Player : public Mob {
public:
    /// @brief Initialize the player with x, y, and speed.
    ///
    /// @param x X position
    /// @param y Y Position
    /// @speed The speed, by default 1.0f.
    Player(std::string username, float x, float y, float speed = 1.0f);
    /// @brief Render the player.
    void render() const;
    /// @brief Update logic and variables, like position n shit
    void tick();
    /// @brief Clone the player
    Player * clone() const;
    /// @brief Return the player's m_username.
    std::string getUsername() const;

    /// @brief Get the combat weapon the player has equipped.
    ///
    /// It returns a pointer to a const BaseWeapon.
    BaseWeapon * getCombatWeapon();

    /// @brief Change the combat weapon the player has.
    void setCombatWeapon(BaseWeapon * b);

    /// @brief Return if the player is holding their combat weapon.
    bool holdingCombatWeapon();

    /// @brief Get the special weapon the player has equipped.
    ///
    /// It returns a pointer to a const BaseWeapon.
    BaseWeapon * getSpecialWeapon();

    /// @brief Change the special weapon the player has.
    void setSpecialWeapon(BaseWeapon * b);

    /// @brief Return whether the player is holding their special weapon.
    bool holdingSpecialWeapon();

    /// @brief Get the current weapon the player is holding.
    ///
    /// Returns pointer to a const BaseWeapon.
    BaseWeapon * getCurrentWeapon();

    /// @brief Change whether the player is holding their combat or special.
    void setCurrentWeapon(WeaponSlot slot);

    // The delay between using a weapon
    int weapon_delay = 0;

private:
    /// @brief Check for input form the keyboard.
    void input();
    /// @brief Move the player up.
    void moveUp();
    /// @brief Move the player down.
    void moveDown();
    /// @brief Move the player left.
    void moveLeft();
    /// @brief Move the player right.
    void moveRight();

    std::string m_username = "Player";
    // How many "pixels" the player has walked.
    float m_distanceWalked = 0;

    BaseWeapon * m_combat_weapon = BlankWeapon;
    BaseWeapon * m_special_weapon = BlankWeapon;
    char m_current_weapon = 0;
};
}
