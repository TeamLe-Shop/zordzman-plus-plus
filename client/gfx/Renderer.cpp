#include "common/entity/components/character.hpp"
#include "common/entity/components/render.hpp"
#include "gfx/Renderer.hpp"


using namespace client::drawingOperations;


namespace client {
namespace gfx {


Renderer::Renderer(sys::RenderWindow &window, HUD &hud, Level &level) :
    m_window(window),
    m_hud(hud),
    m_level(level),
    m_have_player_id(false),
    m_player_id(0) {}


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
    glColor3f(1, 1, 1);
}


void Renderer::drawHUD() {
    auto height = m_window.getHeight();
    auto width = m_window.getWidth();
    // Draw the rectangle/box which contains information about the player.
    setColor(m_hud.hud_box.color);
    drawRectangle(m_hud.hud_box.x, m_hud.hud_box.y, m_hud.hud_box.width,
                  m_hud.hud_box.height, true);
    setColor(m_hud.font_color);

    if (m_have_player_id) {
        // Format the health string & weapon strings
        entity::Entity & player = m_level.m_entities.get(m_player_id);
        auto character = COMPONENT(player, entity::CharacterComponent);
        drawText("default", fmt::format("HP: {}", character->m_health.get()),
                 0, 0 + height - 32, 16, 16);
        drawText("default", "WEP:", 0, 0 + height - 32 + 16, 16, 16);

        // Draw the names of the weapons as smaller components

        // Line border to seperate the actual game from the HUD
        setColor(m_hud.border.color);
        drawRectangle(m_hud.border.x, m_hud.border.y, m_hud.border.width,
                      m_hud.border.height);
    }

    glColor3f(1, 1, 1);
    std::string serverstr =
        fmt::format("Server: {}", m_server_name);
    std::string mapstr = fmt::format("Map: {}", m_map_name);
    drawText("default", serverstr, width - (8 * serverstr.size()),
             height - 8, 8, 8);
    drawText("default", mapstr, width - (8 * mapstr.size()), height - 16, 8, 8);
}


void Renderer::operator()(
        entity::EntityCollection * coll, entity::Entity & ent) {
    auto render    = COMPONENT(ent, entity::RenderComponent);
    auto character = COMPONENT(ent, entity::CharacterComponent);

    auto spriteinfo = render ? render->m_sprite.get() : "";
    auto alphainfo = render ? render->m_alpha.get() : 1.f;

    glColor4f(1, 1, 1, alphainfo);
    drawSprite(spriteinfo, 40, 40, 32, 32);
    glColor4f(1, 1, 1, 1);
}


}  // namespace gfx
}  // namespace client
