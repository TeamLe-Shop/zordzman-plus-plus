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

    host = value(json["host"], "localhost").string_value();
    port = value(json["port"], 4544).int_value();
    name = value(json["name"], "Player").string_value();
    level_dir = value(json["level_dir"], "levels").string_value();
    hud = value(json["hud"], "default_hud.json").string_value();
}

}
