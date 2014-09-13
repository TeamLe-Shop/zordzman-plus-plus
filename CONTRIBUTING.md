Contributing
============

Minimum [clang](https://github.com/llvm-mirror/clang) version: 3.2

Minimum [GCC](https://gcc.gnu.org/gcc-4.7) version: 4.7


### Including OpenGL headers ###

You must always use `<SDL_opengl.h>` instead of `<GL/gl.h>`, as the latter is
not (always?) present on OS X.

### C++11 features that can't be used because gatsan's old compiler ###
- Inheriting constructors
