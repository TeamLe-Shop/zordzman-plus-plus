#pragma once

#include "client/gfx/hud/Element.hpp"

#include "client/gfx/drawingOperations.hpp"

#include <string>

namespace client {

namespace gfx {

using namespace drawingOperations;

class Button : public Element {
public:
    Button(float x, float y, float width, float height, std::string text,
           int textsize, Color c);

    void render();

    void setText(std::string text);
    void setTextSize(int size);

    std::string m_text;
    int m_textsize;
    Color m_color;
};

} // namespace client
} // namespace gfx
