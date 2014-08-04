#include "globalResources.hpp"

#include <unordered_map>
#include <string>

namespace globalResources {

std::unordered_map<std::string, SpriteSheet> sheets;

void init()
{
    sheets.emplace(std::piecewise_construct,
                   std::forward_as_tuple("main"),
                   std::forward_as_tuple("resources/spritesheet.png", 8));
}

const SpriteSheet &getSheet(const char * const key)
{
    auto pair = sheets.find(key);

    if (pair == sheets.end()) {
        throw std::runtime_error("Fuck!");
    }

    return pair->second;
}

void free() {
    sheets.clear();
}

}
