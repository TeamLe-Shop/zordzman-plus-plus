#include "language.hpp"

#include "common/zjson/zjson.hpp"
#include "common/util/debug.hpp"

namespace client {
namespace language {

void setLanguage(std::string language) {
    bool failed = false;
    keyValues = zjson::load(fmt::format("config/{}.json", language), failed);

    if (failed) {
        common::util::debug("Failed to open language file config/{}.json",
                            language);
        current_language = "none";
        return;
    }

    current_language = language;
}

std::string translate(std::string word) {
    if (keyValues[word].dump() != "null") {
        return keyValues[word].string_value();
    } else {
        common::util::debug("Couldn't find key \"{}\" in language \"{}\"",
                            word, current_language);
        return word;
    }
}

} // namespace client
} // namespace language
