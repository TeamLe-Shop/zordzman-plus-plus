zordzman-plus-plus
==================

The best game that ever existed. Period.

Zordzman-plus-plus (Zordzman++) will be a remake of
the original Zordzman created by JavaCakess in, well, Java.
This remake shall be in C++, and will be developed by a team
rather than one person.

Building
========

There are build instructions for Mac and Linux, as well as required libraries below.

Our good friend crumblingstatue (https://github.com/crumblingstatue) has added CMakeLists.txt
to take care of dependencies for us. You'll need to install CMake, either from the command line or 
here: http://www.cmake.org/cmake/resources/software.html

You can also use a Makefile. OSX_Makefile is one that works for OSX. It assumes you have Xcode, though!
Optionally just use CMake.

We don't have a Makefile just for Linux right now. If you're on Linux, just install CMake.
It then goes something like
```
cd zorzdman-plus-plus/
mkdir build
cd build/
cmake ..
cd ..
make
```

Note this will generate some files like CMakeCache.txt and all that. Make sure not to push those up
to the repository!

Libraries
=========

It uses SDL, SDL_Image and OpenGL for about everything right now.
Feel free to... Tag Along!


SDL: https://www.libsdl.org/download-2.0.php (Recommended: 2.0.0)

SDL-Image: https://www.libsdl.org/projects/SDL_image/ (Recommended: 2.0.0)

Mac OS X
========

Download SDL2 and SDL2_image.

Then, after you've opened your SDL2 .dmg, copy the SDL2.framework and
move it into /Library/Frameworks directory. You can find this by navigating to Macintosh HD (usually by double clicking the icon on the desktop) and going to /Library/Frameworks.
Then, do the same with the SDL2_image dmg.

The next part requires XCode (https://developer.apple.com/xcode/downloads/), and what you need to do is find your XCode.app, right click, select "Show Package Contents" and 
you'll be brought to the folder "Contents".
Navigate to Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.x.sdk/Developer/System/Library/Frameworks
and copy OpenGL.framework (It's a folder, remember!) into /Library/Frameworks (back where you put the SDL stuff).

That's about it. It's can be a little tedious, but once that's all set up you're ready to move on!

Then navigate back to the zordzman-plus-plus folder where the Makefile.OSX is and run ```make -f Makefile.OSX```.
Then, run ```./zordzman``` and the game should be brought up!

Some info, there's something a little different with the headers used to #include OpenGL on Mac and other OSes.

You use
```cpp
#include <OpenGL/gl.h>
```
instead of the normal
```cpp
#include <GL/gl.h>
// or
#include <gl.h>
```

Using
```cpp
#include <SDL_opengl.h>
```
instead takes care of this for us.


Linux
=====

http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php is a nice little tutorial on how to set up SDL on linux.

For Linux, you have to download the source code, then do the whole ```./configure ; make all ; make install``` stuff we all love.
Also do the same with SDL_image, though I'm not sure. We'll try our best to make collaborating on Zordzman with any OS a good experience!
