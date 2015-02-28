#include "zjson.hpp"
#include "json11.hpp"

#include <fstream>
#include "format.h"
#include <string>

namespace zjson {

using namespace json11;

Json load(std::string hud, bool & failed) {
    std::ifstream hudfile(hud);

    // Throw a runtime error if file not found.
    if (!hudfile.is_open()) {
        failed = true;
    }

    std::string line;
    std::string jsonStr;

    // Ignore lines starting with '#'.
    while (getline(hudfile, line)) {
        int start = 0;
        while (isspace(line[start])) {
            start++;
        }
        if (line[start] == '#') {
            continue;
        }
        jsonStr += line;
    }

    hudfile.close();

    std::string err;
    return Json::parse(jsonStr, err);
}

} // namespace zjson
