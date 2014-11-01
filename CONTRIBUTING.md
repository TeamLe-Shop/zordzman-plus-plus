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

### Including OpenGL headers ###

You must always use `<SDL_opengl.h>` instead of `<GL/gl.h>`, as the latter is
not (always?) present on OS X.

### C++11 features that can't be used because gatsan's old compiler ###
- Inheriting constructors
