#pragma once

#include <vector>
#include <fstream>

namespace common {
namespace util {
namespace stream {
std::vector<char> readAll(std::istream & stream);
}
}
}
