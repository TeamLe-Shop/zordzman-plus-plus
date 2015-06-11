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


} // namespace language
} // namespace client
