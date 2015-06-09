#pragma once

#include <string>

#include "common/extlib/json11/json11.hpp"

namespace client {

using namespace json11;

/// The configuration structure that holds
///  different config variables.
class Config {

public:
    Config(std::string config);

    template <typename T>
    Json value(Json new_value, T default_value) {
        return new_value.dump() == "null" ? Json { default_value } : new_value;
    }

// You can make this private if you want to add more complications.
public:
    std::string host;
    int port = 4544;
    std::string name;
    std::string level_dir;
};

} // namespace client
