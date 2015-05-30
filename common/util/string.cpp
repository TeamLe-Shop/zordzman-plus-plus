#include "string.hpp"

#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "common/extlib/cppformat/format.h"

namespace common {
namespace util {
namespace string {

void utf8_pop_character(std::string & string) {
    if (string.empty()) {
        return;
    }
    size_t characters_to_pop = 0;
    for (size_t i = string.size() - 1; ; i--) { // Infinitely decrease `i` until
                                                // loop is broken
        ssize_t size = mbstowcs(nullptr, string.c_str() + i, string.size());
        characters_to_pop++;
        if (size == 1) {
            break;
        }
    }

    for (size_t i = 0; i < characters_to_pop; i++) {
        string.pop_back();
    }
}

}
}
}
