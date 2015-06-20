#pragma once

#include <cstddef>
#include <string>

#include "client/HUD.hpp"
#include "client/gfx/drawingOperations.hpp"
#include "client/level/Level.hpp"
#include "client/sys/RenderWindow.hpp"
#include "common/entity/entity.hpp"


namespace client {
namespace gfx {

class Renderer {
public:
    Renderer(sys::RenderWindow &window, HUD &hud, Level &level);
    void render();
    void setPlayerID(unsigned int id);
    void clearPlayerID();
    void setServerName(std::string name);
    void setMapName(std::string map);
    void addNetworkData(std::size_t messages_recieved);
    void operator()(entity::EntityCollection * coll, entity::Entity & ent);

private:
    sys::RenderWindow &m_window;
    HUD &m_hud;
    Level &m_level;
    bool m_have_player_id;
    unsigned int m_player_id;
    std::string m_server_name;
    std::string m_map_name;
    std::size_t m_graph_data_max = 150;
    std::vector<std::size_t> m_graph_data;

    void drawHUD();
    void drawNetGraph();
};

}  // namespace gfx
}  // namespace client
