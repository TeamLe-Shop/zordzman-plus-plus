#pragma once

#include <string>

#include "json11.hpp"

using json11::Json;

class HUD {
public:
    HUD(std::string hudfile);

    void setcol(uint32_t &i, Json json, int base = 10);
    void setint(int &i, Json json, int base = 10);
public:
    struct hud_box {
        int x = 0;
        int y = 568;
        int width = 800;
        int height = 600;
        uint32_t color = 0x515151FF;
    } hud_box;

    struct border {
        int x = 0;
        int y = 567;
        int width = 800;
        int height = 2;
        uint32_t color = 0x00007FFF;
    } border;
};