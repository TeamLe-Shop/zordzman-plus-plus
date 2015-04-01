#include "MusicResource.hpp"

namespace resources {

MusicResource::MusicResource() : m_valid(false) {}

MusicResource::MusicResource(std::string path) : m_path(path), m_valid(true) {}

MusicResource MusicResource::constructFromJson(json11::Json json) {
    return MusicResource(json["path"].string_value());
}

std::string MusicResource::getTypeName() {
    return "music";
}

}
