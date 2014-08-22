#pragma once
#include <string>

namespace json {

/// @brief Return a formatted JSON string.
///
/// @param jsonString The string to format
/// @param specialQuotes Whether to convert '`' to '"' (default = true)
std::string formatJson(std::string jsonString, bool specialQuotes = true);

} // namespace json
