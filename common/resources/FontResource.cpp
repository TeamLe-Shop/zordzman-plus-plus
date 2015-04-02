#include "FontResource.hpp"

namespace resources {

FontResource::FontResource() : m_valid(false) {}

FontResource::FontResource(std::string path, size_t size) : m_path(path),
                                                            m_size(size),
                                                            m_valid(true) {}

FontResource FontResource::constructFromJson(json11::Json json) {
    return FontResource(json["path"].string_value(), json["size"].int_value());
}

std::string FontResource::getTypeName() {
    return "font";
}

}
