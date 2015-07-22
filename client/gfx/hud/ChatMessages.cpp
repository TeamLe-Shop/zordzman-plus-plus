#include "client/gfx/hud/ChatMessages.hpp"

#include "client/gfx/drawingOperations.hpp"

#include <format.h>

namespace client {

namespace gfx {

void ChatMessages::update_height() {
    m_height = m_font_size * m_messages.size();
}

ChatMessages::ChatMessages(float x, float y, size_t maxmsg, size_t font_size) {
    m_x = x;
    m_y = y;
    m_font_size = font_size;
    m_messages.reserve(maxmsg);
    m_messages.resize(0);
    update_height();
}

void ChatMessages::addMessage(std::string msg) {
    m_lastMessage = SDL_GetTicks();
    if (m_messages.size() == m_messages.capacity()) {
        std::move(m_messages.begin() + 1, m_messages.end(),
                  m_messages.begin());
        m_messages[m_messages.size() - 1] = {msg, m_lastMessage};
    } else {
        m_messages.push_back({msg, m_lastMessage});
    }
    update_height();
}

void ChatMessages::render() {
    using namespace drawingOperations;
    m_currentTime = SDL_GetTicks();
    if (m_currentTime > m_lastMessage + 5000
            && m_messages.size() > 0) {
        std::move(m_messages.begin() + 1, m_messages.end(),
                  m_messages.begin());
        m_messages.resize(m_messages.size() - 1);
        m_lastMessage = m_currentTime;
        update_height();
    }

    for (size_t i = 0; i < m_messages.size(); i++) {
        glColor4f(0.2, 0.2, 0.2, 0.3);
        size_t len = mbstowcs(NULL, m_messages[i].message.c_str(), 0);
        drawRectangle(m_x, m_y + i * m_font_size, len * m_font_size,
                      m_font_size);
        glColor4f(1, 1, 1, 1);
        drawText("default", m_messages[i].message, m_x, m_y + i * m_font_size,
                 m_font_size, m_font_size);
    }
}

} // namespace client
} // namespace gfx
