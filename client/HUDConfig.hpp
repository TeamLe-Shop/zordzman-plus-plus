#pragma once

#include <string>

#include "json11.hpp"

using json11::Json;

/// @brief The HUD. This contains various variables
///        for how to render the HUD.
class HUD {
public:
    /// @brief Load a HUD file. HUD files are always in json.
    ///
    /// @param hudfile The file. Hurr durr?
    HUD(std::string hudfile);

    /// @brief Set a color variable.
    ///
    /// @param i The color variable. This function modifies it directly.
    /// @param json The json object. This is expected to be a string of hex.
    /// @param base The base number. The default value is 10.
    void setcol(uint32_t &i, Json json, int base = 10);
    /// @brief Set an integer variable.
    ///
    /// @param i The integer variable. This function modifies it directly.
    /// @param json The json object, which is expected to be an integer.
    /// @param base The base number, by default 10.
    void setint(int &i, Json json, int base = 10);
public:
    /// @brief The font color used when an item is currently selected.
    ///        By default, this is green, or (0x00FF00FF).
    uint32_t font_color_active = 0x00FF00FF;
    /// @brief The normal font color used. By default, it is gray.
    uint32_t font_color = 0xB2B2B2FF;

    /// @brief The box usually containing the HUD.
    ///
    /// x: x position. By default (0).
    /// y: y position. By default (568).
    /// width: The width of the box. By default (800).
    /// height: The height of the box. By default (600).
    /// color: The color, by default a dark gray.
    struct hud_box {
        int x = 0;
        int y = 568;
        int width = 800;
        int height = 600;
        uint32_t color = 0x515151FF;
    } hud_box;

    /// @brief The border meant to seperate the HUD from the
    ///        actual gameplay.
    ///
    /// x: The x position, by default (0).
    /// y: The y position, by default (567).
    /// width: The width, by default (800).
    /// height: The height, by default (2).
    /// color: The color, by default a dark blue.
    struct border {
        int x = 0;
        int y = 567;
        int width = 800;
        int height = 2;
        uint32_t color = 0x00007FFF;
    } border;
};