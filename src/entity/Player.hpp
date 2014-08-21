#pragma once

#include "Entity.hpp"
#include "Mob.hpp"

using namespace mob;

class Player : public Mob {
public:
    /// @brief Initialize the player with x, y, and speed.
    ///
    /// @param x X position
    /// @param y Y Position
    /// @speed The speed, by default 1.0f.
    Player(std::string username, float x, float y, float speed = 1.0f);
    /// @brief Render the player.
    void render();
    /// @brief Update logic and variables, like position n shit
    void tick();
    /// @brief Clone the player
    Player *clone() const;
    /// @brief Return the player's m_m_m_username.
    std::string getUsername();

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
};
