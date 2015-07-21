#include "Level.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "common/util/stream.hpp"

#include "systems/physicsSystem.hpp"

#include "common/entity/components/character.hpp"
#include "common/entity/components/position.hpp"
#include "common/entity/components/render.hpp"
#include "common/entity/component.hpp"

namespace server {

namespace level {

using namespace common::util;

void Level::loadLevel(std::string map_name) {
    name = map_name;
    std::ifstream file(map_name, std::ios::in | std::ios::binary);
    std::vector<char> data = stream::readToEnd(file);
    size = data.size();
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

std::vector<entity::StateChange> Level::cycle() {
    entities.cycle();
    return entities.collectStateChanges();
}

std::vector<entity::StateChange> Level::poll() {
    return entities.poll();
}

unsigned int Level::addPlayer(std::string name) {
    entity::Entity & ent = entities.createEntity();
    ent.addComponent(new entity::CharacterComponent(name, 100));
    ent.addComponent(new entity::PositionComponent(m_spawn_x, m_spawn_y));
    ent.addComponent(new entity::RenderComponent("player_south", 1, 32, 32));
    return ent.getID();
}

void Level::removePlayer(Client & client) {
    entities.removeEntity(client.m_playerID);
}

entity::Entity & Level::getEntity(unsigned int id) {
    return entities.get(id);
}

} // namespace level

} // namespace server
