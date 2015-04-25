#include <string>
#include "format.h"
#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netdb.h>
#include <fcntl.h>
#endif

namespace common {
namespace util {
namespace net {

std::string ipaddr(struct sockaddr_in const addr, bool include_port = false);

void resolvehost(struct sockaddr_in addr, std::string host);

} // namespace net
} // namespace util
} // namespace common
