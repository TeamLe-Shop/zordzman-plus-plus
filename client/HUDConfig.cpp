#include "HUDConfig.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include "json11.hpp"

using namespace json11;

char* trim(char* string) {
    while (isspace(*string)) string++;
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
        if (line[0] == '#') continue;
        jsonStr += line;
    }

    std::string err;
    auto json = Json::parse(jsonStr, err);

    auto bg_color_str = json["bg-color"];
    bg_color = std::stoul(bg_color_str.string_value(), nullptr, 16);

    auto hud_border_str = json["hud-border"];
    border_color = std::stoul(hud_border_str.string_value(), nullptr, 16);
}
