#include "HUDConfig.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include "json11.hpp"

using namespace json11;

char *trim(char *string) {
    while (isspace(*string))
        string++;
    return string;
}

HUD::HUD(std::string hud) {
    std::ifstream hudfile(hud);

    if (!hudfile.is_open()) {
        std::string error("Error loading hud file ");
        error += hud;
        throw std::runtime_error(error);
    }

    std::string line;
    std::string jsonStr;

    while (getline(hudfile, line)) {
        if (line[0] == '#')
            continue;
        jsonStr += line;
    }

    std::string err;
    auto json = Json::parse(jsonStr, err);

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
}

void HUD::setcol(uint32_t &i, Json json, int base) {
    auto x = json;
    if (x.dump() != "null") {
        i = std::stoul(x.string_value(), nullptr, base);
    }
}

void HUD::setint(int &i, Json json, int base) {
    auto x = json;
    if (x.dump() != "null") {
        i = std::stoul(x.dump(), nullptr, base);
    }
}