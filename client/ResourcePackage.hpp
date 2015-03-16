#pragma once

#include <string>

#include <json11.hpp>

namespace client {

enum PackageType {
    Base, ServerProvided, UserProvided
};

class ResourcePackage {
public:
    /// Load a resource package from disk.
    ResourcePackage(std::string path, PackageType type);

    std::vector<json11::Json> getJsonManifest();
    std::string getName();
    PackageType getType();

private:
    std::string m_name;
    PackageType m_type;
    json11::Json m_manifest;
};

}
