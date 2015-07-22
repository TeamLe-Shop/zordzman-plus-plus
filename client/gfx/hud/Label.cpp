#include "client/gfx/hud/Label.hpp"

#include "client/gfx/drawingOperations.hpp"

#include <format.h>

namespace client {

namespace gfx {

Label::Label(float x, float y, std::string text, int size) {
    m_x = x;
    m_y = y;
    m_text = text;
    m_size = size;
    m_width = text.size() * size;
    m_height = size;
}

void Label::setText(std::string text) {
    m_text = text;

    m_width = m_text.size() * m_size;
}

void Label::setSize(int size) {
    m_size = size;

    m_width = m_text.size() * size;
    m_height = size;
}

void Label::render() {
    drawingOperations::drawText("default", m_text, m_x, m_y, m_size, m_size);
}

} // namespace client
} // namespace gfx
