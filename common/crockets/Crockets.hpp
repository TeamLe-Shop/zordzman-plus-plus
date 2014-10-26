#pragma once

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
  #define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
  #define PLATFORM PLATFORM_MAC
#else
  #define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS
  #pragma comment( lib, "wsock32.lib" )
#endif

namespace crockets {

/// @brief Initialize the socket layer.
///        This does not do anything on OS X or Unix/Linux, it is only for
///        Windows systems. This can still be called safely.
bool initializeSockets();

/// @brief Clean up sockets. This doesn't do anything on systems other than
///        Windows.
bool cleanUp();

} // namespace crockets