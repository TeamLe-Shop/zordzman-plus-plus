#pragma once

#include <vector>
#include <fstream>

namespace common {
namespace util {

std::vector<char> readAllFromStream(std::istream & stream) {
    std::vector<char> data;
    auto size = stream.tellg();
    data.resize(size);
    stream.seekg(0, std::ios::beg);
    stream.read(data.data(), size);
    return data;
}
}
}