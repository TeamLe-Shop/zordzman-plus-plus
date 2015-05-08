#include "string.hpp"

#include <string.h>
#include <assert.h>
#include <stdlib.h>

namespace common {
namespace util {
namespace string {

size_t mbstrlen(std::string str) {
    size_t len = 0;
    int pos = 0;
    for (;;) {
        pos += mblen(str.data() + pos, MB_CUR_MAX);
        if (pos < 1) {
            break;
        }
        assert(pos > 0);
        if ((size_t)pos >= MB_CUR_MAX) {
            break;
        }
        ++len;
    }
    return len + 1;
}


}
}
}
