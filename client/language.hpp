#pragma once

#include <string>
#include "common/extlib/json11/json11.hpp"

namespace client {
namespace language {

// Oh my god, it's a global.
static std::string current_language = "none";
static json11::Json keyValues;

void setLanguage(std::string language);

std::string translate(std::string word);

/* HUD */
static std::string Key_Weapon("Weapon");
static std::string Key_Health("Health");
static std::string Key_Map("Map");
static std::string Key_Server("Server");
static std::string Key_Say("Say");
/* Player status messages */
static std::string Key_PlayerJoined("Player \"{}\" joined the game.");
static std::string Key_PlayerLeft("Player \"{}\" left the game.");
static std::string Key_NickTaken("Nickname already taken.");
static std::string Key_NickChange("\"{}\" changed name to \"{}\".");

} // namespace language
} // namespace client
