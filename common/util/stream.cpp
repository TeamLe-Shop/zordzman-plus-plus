#include "stream.hpp"

namespace common {
namespace util {
namespace stream {
std::vector<char> readAll(std::istream & stream) {
    std::vector<char> data;
    auto originalPos = stream.tellg();
    stream.seekg(0, std::ios::end);
    auto end = stream.tellg();
    auto size = end - originalPos;
    data.resize(size);
    stream.read(data.data(), size);
    stream.seekg(originalPos);
    return data;
}
}
}
}
