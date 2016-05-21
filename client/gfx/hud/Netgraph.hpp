#pragma once

#include "client/gfx/hud/Element.hpp"

#include <vector>
#include <cstddef>
#include <cstdint>

namespace client {

namespace gfx {

class Netgraph : public Element {
public:
    // This only exists so we are not forced to use member initialisation
    // in the Renderer constructor.
    Netgraph();

    Netgraph(float x, float y, float width, float height, uint32_t color);

    void render();

    void setWidth(int width);

    void addData(size_t data);

    std::vector<size_t> m_graph_data;
    size_t m_color;
};

} // namespace client
} // namespace gfx
