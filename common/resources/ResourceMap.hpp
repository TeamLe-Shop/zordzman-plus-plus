#pragma once

#include <string>
#include <unordered_map>

#include "ResourcePackage.hpp"

namespace resources {

template <class T>
class ResourceMap {
public:
    ResourceMap(ResourcePackage package) {
        m_name = package.getName();
        m_type = package.getType();

        for (json11::Json j : package.getJsonManifest()) {
            if (j["type"].string_value() == T::getTypeName()) {
                addFromJson(j);
            }
        }
    }

    void add(std::string name, T resource) {
        m_resources.emplace(std::piecewise_construct,
                            std::forward_as_tuple(name),
                            std::forward_as_tuple(resource));
    }

    void addFromJson(json11::Json json) {
        add(json["name"].string_value(), T::constructFromJson(json));
    }

    T lookup(std::string name) {
        for (auto a : m_resources) {
            if (a.first == name) {
                return a.second;
            }
        }

        return T();
    }

    std::unordered_map<std::string, T> getResources() { return m_resources; }

    std::string getName() { return m_name; }

    PackageType getType() { return m_type; }

private:
    std::string m_name;
    std::unordered_map<std::string, T> m_resources;
    PackageType m_type;
};

}

