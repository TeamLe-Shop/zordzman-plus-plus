#pragma once

namespace client {

namespace gfx {

using Response = void (*)();

class Element {
public:
    Element();
    Element(float x, float y);

    void render();

    float m_x, m_y, m_width, m_height;
};

} // namespace gfx
} // namespace client
