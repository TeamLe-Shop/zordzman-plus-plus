#pragma once

#include <string>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#include <unistd.h>

#include "common/net/message.hpp"

using namespace net;

#define REQUIRES(...) typename std::enable_if<(__VA_ARGS__), int>::type = 0

namespace client {
namespace sys {
/// A TCPSocket object one can use to send and receive data.
class TCPSocket {
public:
    /// Connect to a host.
    ///
    /// @param host The host name of the server.
    /// @param portnum The port number.
    ///
    /// @return Whether or not connecting to the host was successful.
    bool connectToHost(std::string host, int portnum);
    /// Receive data from the host.
    ///
    /// @return Received data.
    std::string read();
    /// Send a string to the host.
    ///
    /// @param buf The string to send.
    ///
    /// @return If sending the data was successful.
    bool send(std::string buf);
    /// Send data to the host.
    ///
    /// @param The pointer to the data.
    /// @param The amount of bytes to send.
    ///
    /// @return If the sending was successful.
    bool send(const void * buf, size_t len);
    /// Send numeric data to the host
    ///
    /// @param data - The number to send
    ///
    /// return If the sending was successful.
    template <typename T, REQUIRES(std::is_arithmetic<T>())>
    bool send(T const & data) {
        return send(&data, sizeof(T));
    }
    /// Close the socket.
    void close();
    /// Close the socket when destroyed.
    ~TCPSocket();
    /// Get the IP of the server
    sockaddr_in getServerAddress();
    /// @brief Return IP address of server formatted
    std::string getFormattedServerAddr();

private:
    // The address of the server.
    sockaddr_in m_server;
    // The actual socket.
    Socket m_socket;
    // Our address?
    sockaddr_in m_address;
    // Whether it is open or not.
    bool m_open = false;
};

} // namespace sys
} // namespace client
