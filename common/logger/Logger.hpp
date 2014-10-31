#pragma once

#include <format.h>
#include <functional>

namespace common {

class Logger {
public:
    Logger(FILE * fd = stderr, std::function<std::string()> prefix = no_prefix);
    template <typename... Args>
    /// @brief Prints out formatted text with a prefix to `m_fd`
    ///        (file descriptor)
    void log(std::string const & format, Args... args) {
        fmt::print(m_fd, "{}{}\n", m_prefix(), fmt::format(format, args...));
    }

private:
    FILE * m_fd;
    std::function<std::string()> m_prefix;
    static std::string no_prefix();
};
} // namespace common
