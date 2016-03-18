#include "client/gfx/hud/Button.hpp"

#include "client/gfx/drawingOperations.hpp"

#include "client/gfx/hud/Label.hpp"

#include <format.h>

namespace client {

namespace gfx {

using namespace drawingOperations;

Button::Button(float x, float y, float width, float height, std::string text,
               int textsize, Color c) {
    m_x = x;
    m_y = y;
    m_text = text;
    m_textsize = textsize;
    m_width = width;
    m_height = height;
    m_color = c;
}

void Button::setText(std::string text) {
    m_text = text;
}

void Button::setTextSize(int size) {
    m_textsize = size;
}

void Button::render() {
    int centerx = m_x + (m_width / 2) - (m_text.size() * m_textsize) / 2;
    int centery = m_y + (m_height / 2) - (m_textsize) / 2;

    drawingOperations::setColor(m_color);
    drawingOperations::drawRectangle(m_x, m_y, m_width, m_height, true);
    drawingOperations::setColor(m_color * 0.8);
    drawingOperations::drawRectangle(m_x, m_y + m_height, m_width,
                                     m_height/3, true);
    drawingOperations::setColor(m_color * 1.1);
    drawingOperations::drawRectangle(m_x, m_y, m_width, m_height, false);

    drawingOperations::setColor(255, 255, 255, 255);

    gfx::Label title(centerx, centery, m_text, m_textsize);
    title.render();
}

} // namespace client
} // namespace gfx
