#pragma once

#include <string>
#include <unordered_map>

#include <json11.hpp>

namespace client {

template <class T>
class ResourceCollection {
public:
    void add(std::string name, T resource) {
        m_map.emplace(std::piecewise_construct, std::forward_as_tuple(name),
                    std::forward_as_tuple(resource));
    }

    void addFromJson(json11::Json json) {
        add(json["name"].string_value(), T::constructFromJson(json));
    }

    T lookup(std::string name) {
        for (auto entry : m_map) {
            if (entry.first == name) {
                return entry.second;
            }
        }
        return T();
    }

    T operator[](std::string name) {
        return lookup(name);
    }

    std::unordered_map<std::string, T> map() { return m_map; }

private:
    std::unordered_map<std::string, T> m_map;
};

}
