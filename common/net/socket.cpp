#include "socket.hpp"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace common {
namespace net {

int Socket::getHandle() const { return m_handle; }

Socket::Socket() : m_handle(-1) {}

Socket::Socket(int handle) : m_handle(handle) {}

Socket::~Socket() {
    if (m_handle != -1) {
#ifdef _WIN32
        closesocket(m_handle);
#else
        close(m_handle);
#endif
    }
}

Socket::Socket(Socket && other) {
    m_handle = other.m_handle;
    other.m_handle = -1;
}

Socket & Socket::operator=(Socket && other) {
    m_handle = other.m_handle;
    other.m_handle = -1;
    return *this;
}
}
}
