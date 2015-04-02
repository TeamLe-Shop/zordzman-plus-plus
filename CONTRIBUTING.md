Contributing
============

Minimum [clang](https://github.com/llvm-mirror/clang) version: 3.2

Minimum [GCC](https://gcc.gnu.org/gcc-4.7) version: 4.7

Note: zordzman-plus-plus should be expected to build on OS X 10.7 or greater and
up to date Linux systems.
We can not guarantee zordzman-plus-plus works on Windows currently, however we are
working towards support for most operating systems.

You can always help by trying to build zordzman-plus-plus for yourself on Windows
and report any errors or bugs.

### Talk with us! ###

Network: `irc.caffie.net`

Channel: `#munchtoss`

### Pushing your work ###

Make sure any commits you push to master build. Do not forget to add files!

### Including OpenGL headers ###

You must always use `<SDL_opengl.h>` instead of `<GL/gl.h>`, as the latter is
not (always?) present on OS X.

### Adding new libraries ###

Libraries with source available must have their source placed
in `common/extlib/<libraryname>/`.
Example: base64.cpp and base64.hpp are placed in common/extlib/base64.

To make sure they are built with the project, you must add the following lines to `CMakeLists.txt`: (Example library name is "foo")

`add_library(foo common/extlib/foo/foo.hpp common/extlib/foo/foo.cpp)`

`target_link_libraries(foo)`

Near the bottom of CMakeLists, you'll find something that looks like this:

```cmake
target_link_libraries(zordzman
    ...
    ...
)

target_link_libraries(zordzman-server
    ...
    ...
)
```

After "zordzman" or "zordzman-server" is a list of libraries it needs to build.
Assuming the library you are adding is only needed by zordzman-server: (again,
example library "foo")

```cmake
target_link_libraries(zordzman-server
    ...
    ...
    foo
)
```

If you are writing a collection of functions, classes etc. that you wish to be
available to both the client and server, you put the source files in
`common/<libraryname>`.

In `common/util/` there are source files containing various utility functions.
Example: If you were to write a function dealing with files that are to be used
by both the server and client, you would put it in `fileutil.*pp`.


For adding new SDL libraries (SDL\_mixer, SDL\_ttf, etc.), you must first add
a `FindSDL2_<lib>.cmake` to `CMake/Modules/`. You can simply copy paste the
contents of `FindSDL2_image.cmake` and perform the following operations:
(Example: SDL_ttf)

* Replace all instances of "SDL\_image" with "SDL\_ttf"
* Replace all instances of "SDLIMAGE" with "SDLTTF"
* Replace all instances of "SDL2\_image" with "SDL2\_ttf"

Add the following lines to CMakeLists:
(Example: SDL_ttf)

```cmake
# ! Before the linking of zordzman and zordzman-server !
# You'll find a section where lines similar to this are grouped.
find_package(SDL2_ttf REQUIRED)
include_directories(${SDLTTF_INCLUDE_DIR})
```

Inside the `IF(OSX)` block...

```cmake
set(SDLTTF /Library/Frameworks/SDL2_ttf.framework)
set(SDLTTF_INCLUDE_DIR /Library/Frameworks/SDL2_ttf.framework/Headers)
```

Add `${SDLTTF_LIBRARY}` to the list of dependancies of which target you want
the library available (Example: zordzman-server)

```cmake
target_link_libraries(zordzman-server
    ... # Other dependancies...
    ...
    ${SDLTTF_LIBRARY}
)
```

### C++11 features that can't be used because ~~gatsan's old compiler~~ support for clang 3.2 ###
- Inheriting constructors
