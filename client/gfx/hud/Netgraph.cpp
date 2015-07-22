#include "client/gfx/hud/Netgraph.hpp"

#include "client/gfx/drawingOperations.hpp"

#include <format.h>

namespace client {

namespace gfx {

Netgraph::Netgraph() {}

Netgraph::Netgraph(float x, float y, float w, float h, uint32_t color)
        : Element(x, y), m_color(color), m_graph_data(0) {
    m_width = w;
    m_height = h;

    m_graph_data.resize(0);
}

void Netgraph::setWidth(int width) {
    m_width = width;
}

void Netgraph::addData(size_t data) {
    m_graph_data.push_back(data);
    if (m_graph_data.size() > m_width) {
        m_graph_data.erase(m_graph_data.begin());
        m_graph_data.resize(m_width);
    }
}

void Netgraph::render() {
    using namespace drawingOperations;
    glColor4f(0.2f, 0.2f, 0.2f, 0.2f);
    drawRectangle(m_x, m_y, m_width, m_height, true);
    glColor4f(0, 0, 1, 0.9f);
    for (size_t i = 0; i < m_graph_data.size(); i++) {
        if (m_graph_data[i]) {
            drawLine(m_x + i + (m_width - m_graph_data.size()), m_y + m_height,
                     m_x + i + (m_width - m_graph_data.size()),
                     m_y + m_height - m_graph_data[i] * 2);
        }
    }
    glColor4f(1, 1, 1, 1);
}

} // namespace client
} // namespace gfx
