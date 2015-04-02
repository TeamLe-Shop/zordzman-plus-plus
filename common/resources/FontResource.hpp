#pragma once

#include <json11.hpp>
#include <string>

namespace resources {

class FontResource {
public:
    FontResource();
    FontResource(std::string path, size_t size);

    static FontResource constructFromJson(json11::Json json);
    static std::string getTypeName();

    std::string m_path;
    size_t m_size;

    bool m_valid;
};

}
