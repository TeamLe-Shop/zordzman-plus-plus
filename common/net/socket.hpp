#pragma once

namespace common {
namespace net {
class Socket {
public:
    int getHandle() const;
    Socket();
    Socket(int handle);
    ~Socket();
    Socket(Socket const &) = delete;
    void operator=(Socket const &) = delete;
    Socket(Socket && other);
    Socket & operator=(Socket && other);

private:
    int m_handle;
};
}
}
