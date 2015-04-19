#pragma once

#include <string>

#include <json11.hpp>

#include "common/star/tarlib.hpp"


namespace resources {

enum PackageType {
    Base, Map, ServerProvided, UserProvided
};

class ResourcePackage {
public:
    /// Load a resource package from disk.
    ResourcePackage(std::string path, PackageType type);

    std::vector<json11::Json> getJsonManifest();
    std::string getName();
    PackageType getType();
    Tar getTar();

private:
    std::string m_name;
    PackageType m_type;
    Tar m_tar;
    json11::Json m_manifest;
};

}
