#include <string>
#include "format.h"
#include <netinet/in.h>

namespace common {
namespace util {
namespace net {

std::string ipaddr(struct sockaddr_in const addr);

} // namespace net
} // namespace util
} // namespace common
