#include <string>
#include "format.h"
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

namespace common {
namespace util {
namespace net {

std::string ipaddr(struct sockaddr_in const addr, bool include_port = false);

} // namespace net
} // namespace util
} // namespace common
