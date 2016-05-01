#include "client/gfx/hud/Button.hpp"

#include "client/gfx/drawingOperations.hpp"

#include "client/gfx/hud/Label.hpp"

#include <format.h>

namespace client {

namespace gfx {

using namespace drawingOperations;

Button::Button(float x, float y, float width, float height, std::string text,
               int textsize, Color c, Response buttonDown, Response buttonUp) {
    m_x = x;
    m_y = y;
    m_text = text;
    m_textsize = textsize;
    m_width = width;
    m_height = height;
    m_color = c;
    m_buttonDown = buttonDown;
    m_buttonUp = buttonUp;
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

    int mousex, mousey;
    unsigned int mousestate = SDL_GetMouseState(&mousex, &mousey);

    if (mousex > m_x && mousex < m_x + m_width && mousey > m_y
        && mousey < m_y + m_height) { // Mouse over
        if (mousestate & SDL_BUTTON(SDL_BUTTON_LEFT)) { // Pressed
            if (!m_pressed) {
                if (m_buttonDown) { m_buttonDown(); }
                m_pressed = true;
            }
            drawTop(m_color * 1.1, m_y + m_height/3);
            drawOutline(m_y + m_height/3);
            centery += m_height/3;
        } else {
            if (m_pressed) {
                if (m_buttonUp) { m_buttonUp(); }
                m_pressed = false;
            }
            drawTop(m_color * 1.1, m_y);
            drawOutline(m_y);
            drawBottom();
        }
    } else {
        if (m_pressed) {
            if (m_buttonUp) { m_buttonUp(); }
            m_pressed = false;
        }
        drawTop(m_color, m_y);
        drawBottom();
        drawOutline(m_y);
    }

    drawingOperations::setColor(255, 255, 255, 255);

    gfx::Label title(centerx, centery, m_text, m_textsize);
    title.render();
}

void Button::drawTop(Color col, int y) {
    drawingOperations::setColor(col);
    drawingOperations::drawRectangle(m_x, y, m_width, m_height, true);
}

void Button::drawOutline(int y) {
    drawingOperations::setColor(m_color * 1.1);
    drawingOperations::drawRectangle(m_x, y, m_width, m_height, false);
}

void Button::drawBottom() {
    drawingOperations::setColor(m_color * 0.8);
    drawingOperations::drawRectangle(m_x, m_y + m_height, m_width, m_height/3,
                                     true);
}

} // namespace client
} // namespace gfx
