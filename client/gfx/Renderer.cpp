#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"
#include "gfx/Renderer.hpp"

#include "language.hpp"

using namespace client::drawingOperations;

namespace client {
namespace gfx {

Renderer::Renderer(sys::RenderWindow &window, HUD &hud, Level &level) :
        m_window(window),
        m_hud(hud),
        m_level(level),
        m_have_player_id(false),
        m_player_id(0),
        m_chatMessages(10) {

    float graph_width = 300;
    float graph_height = 100;
    m_netgraph = Netgraph(window.getWidth() - graph_width,
                          window.getHeight() - graph_height - 32, graph_width,
                          graph_height, color_float(0, 0, 1, 0.9f));
    m_chatMessages.resize(0);
}

void Renderer::setPlayerID(unsigned int id) {
    m_have_player_id = true;
    m_player_id = id;
}

void Renderer::clearPlayerID() {
    m_have_player_id = false;
    m_player_id = 0;
}

void Renderer::setServerName(std::string name) {
    m_server_name = name;
}

void Renderer::setMapName(std::string name) {
    m_map_name = name;
}

void Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT);  // Clear the screen.
    m_level.render();
    drawHUD();
    m_netgraph.render();
    drawChat();
    glColor3f(1, 1, 1);

    m_currentTime = SDL_GetTicks();
    if (m_currentTime > m_lastMessage + 5000
            && m_chatMessages.size() > 0) {
        std::move(m_chatMessages.begin() + 1, m_chatMessages.end(),
                  m_chatMessages.begin());
        m_chatMessages.resize(m_chatMessages.size() - 1);
        m_lastMessage = m_currentTime;
    }
}

void Renderer::drawHUD() {
    using namespace language;

    auto height = m_window.getHeight();
    auto width = m_window.getWidth();
    // Draw the box which contains information about the player.
    setColor(m_hud.hud_box.color);
    drawRectangle(m_hud.hud_box.x, m_hud.hud_box.y, m_hud.hud_box.width,
                  m_hud.hud_box.height, true);
    setColor(m_hud.font_color);

    if (m_have_player_id && m_level.m_entities.contains(m_player_id)) {
        // Format the health string & weapon strings
        entity::Entity & player = m_level.m_entities.get(m_player_id);
        auto character = COMPONENT(player, entity::CharacterComponent);

        std::string hp_string = fmt::format("{}: {}", translate(Key_Health),
                                            character->m_health.get());
        Label hp_label = Label(0, height - 32, hp_string, 16);
        hp_label.render();
        Label wep_label = Label(0, height - 16,
                                fmt::format("{}:", translate(Key_Weapon)), 16);
        wep_label.render();
    }

    // Line border to seperate the actual game from the HUD
    setColor(m_hud.border.color);
    drawRectangle(m_hud.border.x, m_hud.border.y, m_hud.border.width,
                  m_hud.border.height);

    setColor(m_hud.font_color);

    std::string serverstr =
        fmt::format("{}: {}", translate(Key_Server), m_server_name);
    std::string mapstr = fmt::format("{}: {}", translate(Key_Map), m_map_name);
    Label(width - (8 * serverstr.size()), height - 8, serverstr, 8).render();
    Label(width - (8 * mapstr.size()), height - 16, mapstr, 8).render();

    glColor4f(1, 1, 1, 1);
}

void Renderer::drawChat() {
    using namespace drawingOperations;
    using namespace language;
    auto const height = m_window.getHeight();
    auto const width = m_window.getWidth();
    if (chat_open) {
        if (chat_fade_timer < chat_maxfade) {
            chat_fade_timer++;
        }
    } else {
        if (chat_fade_timer > chat_minfade) {
            chat_fade_timer--;
        }
    }

    if (chat_fade_timer) {
        float fade = (float)chat_fade_timer / (float)chat_maxfade;
        glColor4f(0.3, 0.3, 0.3, fade);
        drawRectangle(0, m_hud.border.y - 9, width, 9, false);
        glColor4f(0.2, 0.2, 0.2, fade);
        drawRectangle(1, m_hud.border.y - 9, width - 1, 8);
        glColor4f(1, 1, 1, fade);
        drawText("default", fmt::format("{}: {}", translate(Key_Say),
                                                  chat_string),
                 0, m_hud.border.y - 9, 8, 8);
    }

    for (size_t i = 0; i < m_chatMessages.size(); i++) {
        glColor4f(0.2, 0.2, 0.2, 0.3);
        size_t len = mbstowcs(NULL, m_chatMessages[i].message.c_str(), 0);
        drawRectangle(0, i * 8, len * 8, 8);
        glColor4f(1, 1, 1, 1);
        drawText("default", m_chatMessages[i].message, 0, i * 8, 8, 8);
    }
}

void Renderer::addMessage(std::string msg) {
    m_lastMessage = SDL_GetTicks();
    if (m_chatMessages.size() == m_chatMessages.capacity()) {
        std::move(m_chatMessages.begin() + 1, m_chatMessages.end(),
                  m_chatMessages.begin());
        m_chatMessages[m_chatMessages.size() - 1] = {msg, m_lastMessage};
    } else {
        m_chatMessages.push_back({msg, m_lastMessage});
    }
}

void Renderer::addNetworkData(size_t data) {
    m_netgraph.addData(data);
}

}  // namespace gfx
}  // namespace client
