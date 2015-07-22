#pragma once

#include "client/gfx/hud/Element.hpp"

#include <string>
#include <vector>

namespace client {

namespace gfx {

typedef struct {
    std::string message;
    uint32_t timestamp;
} ChatMessage;

class ChatMessages : public Element {
public:
    ChatMessages(float x=0, float y=0, size_t max_msg=5, size_t m_font_size=16);

    void render();

    void addMessage(std::string msg);

    void update_height();

    std::vector<ChatMessage> m_messages;
    uint32_t m_lastMessage;
    uint32_t m_currentTime;

    size_t m_font_size;
};

} // namespace client
} // namespace gfx
