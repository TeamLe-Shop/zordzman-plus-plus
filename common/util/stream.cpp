#include "stream.hpp"

namespace common {
namespace util {
namespace stream {
std::vector<char> readToEnd(std::istream & stream) {
    std::vector<char> data;
    auto originalPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    auto end = stream.tellg();
    auto size = end - originalPos;
    data.resize(size);
    stream.seekg(originalPos);
    stream.read(data.data(), size);
    return data;
}
} // namespace stream
} // namespace util
} // namespace common
