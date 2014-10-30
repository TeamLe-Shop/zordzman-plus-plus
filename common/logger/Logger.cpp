#include "Logger.hpp"

namespace common {
Logger::Logger(FILE * fd, std::function<std::string()> prefix)
    : m_fd(fd), m_prefix(prefix) {}

std::string Logger::no_prefix() { return std::string(); }
} // namespace common
