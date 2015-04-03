#pragma once

#include <json11.hpp>
#include <string>

namespace resources {

class SoundResource {
public:
    SoundResource();
    SoundResource(std::string path);

    static SoundResource constructFromJson(json11::Json json);
    static std::string getTypeName();

    std::string m_path;

    bool m_valid;
};

}
