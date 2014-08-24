#pragma once

#include <string>

class HUD {
public:
    HUD(std::string hudfile);

public:
    uint32_t bg_color;
    uint32_t border_color;
};