#include "json11.hpp"

#include <string>
#include <stdexcept>

using namespace json11;

namespace zjson {

/// Load a Zordzman JSON HUD file.
///
/// When reading the file, lines starting with `#` are ignored.
///
/// @param hud The filename of the HUD file.
Json load(std::string hud);
}