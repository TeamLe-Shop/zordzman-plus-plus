#include "json11.hpp"

#include <string>
#include <stdexcept>

namespace zjson {

/// Load a Zordzman JSON HUD file.
///
/// When reading the file, lines starting with `#` are ignored.
///
/// @param hud The filename of the HUD file.
/// @param quit Whether to quit if failed to open file
json11::Json load(std::string hud, bool & failed);
}
