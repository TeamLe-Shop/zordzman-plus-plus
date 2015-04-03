#pragma once

#include <string>
#include <unordered_map>

#include <json11.hpp>

#include "PackageResources.hpp"
#include "ResourcePackage.hpp"

namespace resources {

template <class T>
/// Generic collection of a specific type of resources.
class ResourceCollection {

using Map = std::unordered_map<std::string, T>;

public:
    void loadPackage(ResourcePackage package) {
       m_packages.emplace(m_packages.begin(), PackageResources<T>(package));
    }

    void unloadPackages(PackageType type) {
        for (int i = 0; i < m_packages.size(); i++) {
            PackageResources<T> map = map.get(i);
            if (map.getType() == type) {
                m_packages.erase(m_packages.begin() + i);
            }
        }
    }

    T lookup(std::string name) {
        for (PackageResources<T> map : m_packages) {
            T temp = map.lookup(name);
            if (temp.m_valid) {
                return temp;
            }
        }
        return T();
    }

    T operator[](std::string name) {
        return lookup(name);
    }

    std::vector<PackageResources<T>> getPackages() {
        return m_packages;
    }

private:
    std::vector<PackageResources<T>> m_packages;
};

}
