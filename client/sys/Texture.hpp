#pragma once

#include <string>
#include <SDL_opengl.h>

namespace client {
namespace sys {

/// @brief Texture
class Texture {
public:
    /// @brief Load the texture from a file
    ///
    /// @param filename The name of the file to load from
    bool loadFromFile(std::string const & filename);
    /// @brief Get the width of the texture
    int getWidth() const;
    /// @brief Get the height of the texture
    int getHeight() const;
    /// @brief Bind a texture for subsequent drawing calls
    ///
    /// @param texture The texture to bind
    static void bind(Texture const & texture);
    /// @brief Unbind the currently bound texture
    static void unbind();
    /// @brief Destroy the texture.
    ~Texture();
    /// @brief Construct the texture from a file
    Texture(std::string const & filename);

    // Forbid copying
    Texture(const Texture &) = delete;
    Texture & operator=(const Texture &) = delete;

private:
    GLuint m_handle;
    int m_width, m_height;
};
}
}
