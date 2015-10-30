#pragma once

#include "client/gfx/hud/Element.hpp"

#include "gfx/drawingOperations.hpp"

#include <string>

namespace client {

namespace gfx {

using namespace drawingOperations;

class Header : public Element {
public:
    Header(float x, float y, float w, float h, float bh, Color tcol, Color hcol,
           std::string title, int titlesize);

    void render();

    void setWidth(float width);
    void setHeight(float height);
    void setBottomHeight(float height);
    void setTitle(std::string text);
    void setTitleSize(int size);
    void setTitleColor(Color color);
    void setHeaderColor(Color color);

private:
    std::string m_title;
    int m_titlesize;
    Color m_titlecolor;
    Color m_headercolor;

    float m_x, m_y, m_width, m_height, m_bottom_height;
};

} // namespace client
} // namespace gfx
