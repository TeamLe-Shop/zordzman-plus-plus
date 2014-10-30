#pragma once

#include <vector>
#include <fstream>

namespace common {
namespace util {
namespace stream {
/// @brief Read a stream to the end, return a vector containing the read data.
///
/// The original position of the stream is not restored.
std::vector<char> readToEnd(std::istream & stream);
}
}
}
