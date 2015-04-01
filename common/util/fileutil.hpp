#pragma once

#include <string>
#include <vector>

#include <sys/stat.h>

namespace common {
namespace util {
namespace file {

/// Return the file name in a path string.
std::string fileFromPath(std::string path);

/// Return a vector of the names of the files in a directory.
///
/// @param path The directory path
std::vector<std::string> getDirectoryContents(std::string const & path);

/// Check if a file is a directory
///
/// @param path File path
bool isDirectory(std::string path);

} // namespace stream
} // namespace util
} // namespace common
