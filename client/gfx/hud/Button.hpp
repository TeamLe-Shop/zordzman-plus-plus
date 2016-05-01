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
           int textsize, Color c, Response buttonDown, Response buttonUp);

    void render();

    void setText(std::string text);
    void setTextSize(int size);

    void drawTop(Color col, int y);
    void drawOutline(int y);
    void drawBottom();

    std::string m_text;
    int m_textsize;
    Color m_color;
    bool m_pressed = false;
    Response m_buttonDown, m_buttonUp;
};

} // namespace client
} // namespace gfx
