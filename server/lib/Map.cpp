#include "Map.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "common/util/stream.hpp"

#include "systems/physicsSystem.hpp"

#include "common/entity/components/character.hpp"
#include "common/entity/components/position.hpp"
#include "common/entity/component.hpp"

#include "Client.hpp"

namespace server {

namespace map {

using namespace common::util;

void Level::loadLevel(std::string map_name) {
    name = map_name;
    std::ifstream file(map_name, std::ios::in | std::ios::binary);
    std::vector<char> data = stream::readToEnd(file);
    md5.add(data.data(), data.size());
    m_base64 = base64_encode((byte *)data.data(), data.size());

    // Width, height, spawn X and spawn Y are the first 4 bytes.
    m_width = data[0];
    m_height = data[1];
    m_spawn_x = data[2] * 32;
    m_spawn_y = data[3] * 32;
    // Resize the vector to match the width and height.
    m_tiles.resize(m_width * m_height);
    // Copy over the level data over to the vector.
    std::copy(data.begin() + 4, data.begin() + 4 + m_width * m_height,
              m_tiles.begin());

    entities.addSystem(physicsSystem);
}

std::string Level::asBase64() { return m_base64; }

void Level::cycle() { entities.cycle(); }

unsigned int Level::addPlayer(std::string name) {
    entity::Entity &ent = entities.createEntity();
    ent.addComponent(new entity::CharacterComponent(name, 100));
    ent.addComponent(new entity::PositionComponent(m_spawn_x, m_spawn_y));

   return ent.getID();
}

void Level::removePlayer(Client &client) {
    entities.removeEntity(client.getPlayerID());
}

} // namespace map

} // namespace server
