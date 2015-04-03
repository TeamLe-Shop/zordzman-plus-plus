#pragma once

#include <string>
#include <SDL_opengl.h>
#include <SDL.h>

namespace client {
namespace sys {

enum LoadMethod {
    File, Memory
};

struct TexResult {
    bool ok;
    GLuint handle;
    int width, height;
};

/// Texture
class Texture {
public:
    /// Load the texture from a file
    ///
    /// @param filename The name of the file to load from
    bool loadFromFile(std::string const & filename);
    /// Load texture from memory
    ///
    /// @param memory Memory location
    bool loadFromMemory(std::string const & str);
    /// Get the width of the texture
    int getWidth() const;
    /// Get the height of the texture
    int getHeight() const;
    /// Bind a texture for subsequent drawing calls
    ///
    /// @param texture The texture to bind
    static void bind(Texture const & texture);
    /// Unbind the currently bound texture
    static void unbind();

    static TexResult load_texture(SDL_Surface * surface);
    /// Destroy the texture.
    ~Texture();
    /// Construct the texture from a file
    Texture(std::string const & filename);
    Texture(std::string const & str, LoadMethod lm);
    Texture(TexResult result);

    GLuint m_handle;
private:
    int m_width, m_height;
};
} // namespace sys
} // namespace client
