#include "client/gfx/hud/Header.hpp"

#include "client/gfx/drawingOperations.hpp"

#include "client/Client.hpp"

#include <format.h>

namespace client {

namespace gfx {

using namespace drawingOperations;

Header::Header(float x, float y, float w, float h, float bh, Color hcol,
               Color tcol, std::string titletext, int titlesize) {
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;
    m_bottom_height = bh;
    m_title = titletext;
    m_titlesize = titlesize;
    m_titlecolor = tcol;
    m_headercolor = hcol;
}

void Header::setTitle(std::string text) { m_title = text; }

void Header::setTitleSize(int size) { m_titlesize = size; }

void Header::setWidth(float width) { m_width = width; }

void Header::setHeight(float height) { m_height = height; }

void Header::setBottomHeight(float height) { m_bottom_height = height; }

void Header::setTitleColor(Color color) { m_titlecolor = color; }

void Header::setHeaderColor(Color color) { m_headercolor = color; }

void Header::render() {
    int centerx = m_x + (m_width / 2) - (m_title.size() * m_titlesize) / 2;
    int centery = m_y + (m_height / 2) - (m_titlesize) / 2;

    drawingOperations::setColor(m_headercolor);
    drawingOperations::drawRectangle(m_x, m_y, m_width, m_height, true);
    drawingOperations::setColor(m_headercolor * 0.8);
    drawingOperations::drawRectangle(m_x, m_y + m_height, m_width,
                                     m_bottom_height, true);
    drawingOperations::setColor(m_titlecolor);

    gfx::Label title(centerx, centery, m_title, m_titlesize);
    title.render();
}

} // namespace client
} // namespace gfx
