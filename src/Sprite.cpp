#include "Sprite.hpp"

Sprite::Sprite(Texture const &texture) { setTexture(texture); }

void Sprite::setupQuadSizeInfo() {
    auto const halfWidth = m_texture->getWidth() / 2;
    m_quads[0] = -halfWidth;
    m_quads[1] = -halfWidth;
    m_quads[2] = halfWidth;
    m_quads[3] = -halfWidth;
    m_quads[4] = halfWidth;
    m_quads[5] = halfWidth;
    m_quads[6] = -halfWidth;
    m_quads[7] = halfWidth;
}

void Sprite::draw() {
    glPushMatrix();
    m_texture->bind();
    glTranslatef(m_x + m_texture->getWidth() / 2,
                 m_y + m_texture->getHeight() / 2, 0.0f);
    glRotatef(m_angle, 0.0f, 0.0f, 1.0f);
    glColorPointer(3, GL_FLOAT, 0, m_colors);
    glTexCoordPointer(2, GL_FLOAT, 0, m_texcoords);
    glVertexPointer(2, GL_FLOAT, 0, m_quads);
    glDrawArrays(GL_QUADS, 0, 8);
    glPopMatrix();
}

void Sprite::setAngle(int angle) { m_angle = angle; }

void Sprite::setColor(Uint8 r, Uint8 g, Uint8 b) {
    m_colors[0] = m_colors[3] = m_colors[6] = m_colors[9] = r / 255.0f;
    m_colors[1] = m_colors[4] = m_colors[7] = m_colors[10] = g / 255.0f;
    m_colors[2] = m_colors[5] = m_colors[8] = m_colors[11] = b / 255.0f;
}

void Sprite::setTexture(Texture const &texture) {
    m_texture = &texture;
    setupQuadSizeInfo();
}

void Sprite::setPosition(int x, int y) {
    m_x = x;
    m_y = y;
}
