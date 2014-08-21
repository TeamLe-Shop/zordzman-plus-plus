#include "cJSON.h"
#include <cstring>
#include <cstdlib>
#include "json/json.hpp"

namespace json {

std::string formatJson(std::string jsonString, bool specialQuotes) {
    if (specialQuotes) {
        for (size_t i = 0; i < jsonString.size(); i++) {
            if (jsonString[i] == '`') {
                jsonString[i] = '\"';
            }
        }
    }

    cJSON *json = cJSON_Parse(jsonString.c_str());
    char *out;

    out = cJSON_Print(json);
    cJSON_Delete(json);

    std::string result_string(out);

    free(out);
    return result_string;
}

} // namespace json
