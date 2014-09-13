#include "HUDConfig.hpp"

#include <fstream>
#include <stdexcept>

using namespace json11;

HUD::HUD(std::string hud) {
    std::ifstream hudfile(hud);

    // Throw a runtime error if file not found.
    if (!hudfile.is_open()) {
        std::string error("Error loading hud file ");
        error += hud;
        throw std::runtime_error(error);
    }

    std::string line;
    std::string jsonStr;

    // Ignore lines starting with '#'.
    while (getline(hudfile, line)) {
        int start = 0;
        while (isspace(line[start]))
            start++;
        if (line[start] == '#')
            continue;
        jsonStr += line;
    }

    std::string err;
    auto json = Json::parse(jsonStr, err);

    // Find and set all the variables...
    setint(border.x, json["border"]["x"]);
    setint(border.y, json["border"]["y"]);
    setint(border.width, json["border"]["width"]);
    setint(border.height, json["border"]["height"]);
    setcol(border.color, json["border"]["color"], 16);

    setint(hud_box.x, json["hud-box"]["x"]);
    setint(hud_box.y, json["hud-box"]["y"]);
    setint(hud_box.width, json["hud-box"]["width"]);
    setint(hud_box.height, json["hud-box"]["height"]);
    setcol(hud_box.color, json["hud-box"]["color"], 16);

    setcol(font_color, json["font-color"], 16);
    setcol(font_color_active, json["font-color-active"], 16);
}

void HUD::setcol(uint32_t & i, Json json, int base) {
    auto x = json;
    if (x.dump() != "null") {
        i = std::stoul(x.string_value(), nullptr, base);
    }
}

void HUD::setint(int & i, Json json, int base) {
    auto x = json;
    if (x.dump() != "null") {
        i = std::stoul(x.dump(), nullptr, base);
    }
}
