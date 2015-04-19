#include "Config.hpp"

#include <json11.hpp>

#include "common/zjson/zjson.hpp"
#include "common/util/debug.hpp"

namespace client {

Config::Config(std::string config) {
    bool failed = false;
    json11::Json json = zjson::load(config, failed);

    if (failed) {
        common::util::debug("Failed to open configuration file {}\n", config);
        return;
    }

    host = json["host"].string_value();
    port = json["port"].int_value();
    name = json["name"].string_value();
    level_dir = json["level_dir"].string_value();
}

}
