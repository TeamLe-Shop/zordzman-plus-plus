#pragma once

#include <string>
#include <unordered_map>

#include <json11.hpp>

#include "ResourceMap.hpp"
#include "ResourcePackage.hpp"

namespace client {

template <class T>
/// Generic collection of a specific type of resources.
class ResourceCollection {

using Map = std::unordered_map<std::string, T>;

public:
    void loadPackage(ResourcePackage package) {
       m_packages.add(ResourceMap<T>(package));
    }

    void unloadPackages(PackageType type) {
        for (int i = 0; i < m_packages.size(); i++) {
            ResourceMap<T> map = map.get(i);
            if (map.getType() == type) {
                m_packages.erase(m_packages.begin() + i);
            }
        }
    }

    T lookup(std::string name) {
        for (int i = m_packages.size() - 1; i >= 0; i--) {
            T temp = m_packages.get(m_packages.begin() + i).lookup(name);
            if (temp.valid()) {
                return temp;
            }
        }
        return T();
    }

    T operator[](std::string name) {
        return lookup(name);
    }

    std::vector<T> all() {
        std::vector<T> all;
        for (ResourceMap<T> map : m_packages) {
            for (T r : map.getResources()) {
                all.add(r);
            }
        }
        return all;
    }

private:
    std::vector<ResourceMap<T>> m_packages;
};

}
