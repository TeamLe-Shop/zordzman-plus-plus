#include "fileutil.hpp"

#include <iostream>

namespace common {
namespace util {
namespace file {

/// Return the file name in a path string.
std::string fileFromPath(std::string str) {
    unsigned found = str.find_last_of("/\\");
    return str.substr(found + 1);
}

} // namespace stream
} // namespace util
} // namespace common
