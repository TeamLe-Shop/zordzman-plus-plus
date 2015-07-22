#pragma once

#include "client/gfx/hud/Element.hpp"

#include <string>

namespace client {

namespace gfx {

class Label : public Element {
public:
    Label(float x, float y, std::string text, int size);

    void render();

    void setText(std::string text);
    void setSize(int size);

    std::string m_text;
    int m_size;
};

} // namespace client
} // namespace gfx
