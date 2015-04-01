#pragma once

#include <json11.hpp>
#include <string>

namespace resources {

class MusicResource {
public:
    MusicResource();
    MusicResource(std::string path);

    static MusicResource constructFromJson(json11::Json json);
    static std::string getTypeName();

    std::string m_path;

    bool m_valid;
};

}
