#pragma once



namespace client {
class Level;
class Entity {
public:
    /// @brief Entity Constructor.
    ///
    /// @param x Initial x position
    /// @param y Initial y position
    Entity(float x, float y);
    /// @brief Call the render code for an entity.
    virtual void render() const;
    /// @brief Update logic for an entity.
    virtual void tick();
    virtual ~Entity();
    virtual Entity * clone() const = 0;

    Level * getLevel();
    void setLevel(Level * level);
protected:
    float m_x;
    float m_y;

    Level * m_level;
};
} // namespace client
