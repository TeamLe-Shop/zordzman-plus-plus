#include "SoundResource.hpp"

namespace resources {

SoundResource::SoundResource() : m_valid(false) {}

SoundResource::SoundResource(std::string path) : m_path(path), m_valid(true) {}

SoundResource SoundResource::constructFromJson(json11::Json json) {
    return SoundResource(json["path"].string_value());
}

std::string SoundResource::getTypeName() {
    return "sound";
}

}
