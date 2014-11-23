#pragma once

namespace client {
namespace sys {

/// Provides the context which the other sys components
/// depend on
class SysContext {
public:
    SysContext();
    ~SysContext();
};
} // namespace sys
} // namespace client
