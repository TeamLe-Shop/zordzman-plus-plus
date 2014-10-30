#pragma once

namespace common {

namespace util {
namespace container {
template <typename Container, typename Predicate>
void remove_if(Container & c, Predicate p) {
    c.erase(std::remove_if(c.begin(), c.end(), p), c.end());
}
} // namespace container
} // namespace util
} // namespace common
