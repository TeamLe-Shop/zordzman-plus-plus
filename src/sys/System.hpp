#pragma once

namespace sys {

/// @brief Provides the context which the other sys components
/// depend on
class System {
public:
    /// @brief Initialize SDL
    System();
    /// @brief Quit SDL.
    ~System();
};
}
