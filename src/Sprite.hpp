#pragma once

#include "Texture.hpp"

#include <SDL.h>

class Sprite {
public:
    void setColor(Uint8 r, Uint8 g, Uint8 b);
    void setTexture(Texture const &texture);
    void draw();
    void setPosition(int x, int y);
    void setAngle(int angle);
    Sprite() = default;
    Sprite(Texture const &texture);

private:
    int m_x{ 0 }, m_y{ 0 }, m_angle{ 0 };
    GLfloat m_quads[8];
    GLfloat const m_texcoords[8]{
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    GLfloat m_colors[12]{ 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
                          1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
    Texture const *m_texture;
    /**
     * @brief I have no idea.
     *
     * TODO: Figure out what this shit is or does
     */
    void setupQuadSizeInfo();
};
