zordzman-plus-plus
==================

![](http://servers.tf:8080/buildStatus/icon?job=zordzman-plus-plus)

(Note: The above icon may show the incorrect build status due to GitHub cacheing the images.)

The best game that ever existed!

Zordzman-plus-plus (Zordzman++) will be a remake of
the original Zordzman created by JavaCakess in, well, Java.
This remake shall be in C++, and will be developed by a [team](CONTRIBUTORS.md)
rather than one person.

Dependencies
============

The client uses SDL and other SDL libraries. The server requires none of those
dependencies to be present.

It uses [json11](https://github.com/dropbox/json11) for parsing JSON,
[cppformat](http://cppformat.github.io/) for formatting, and
[hash-library](http://create.stephan-brumme.com/hash-library/) for hashing and
[base64](http://www.adp-gmbh.ch/cpp/common/base64.html) for base64 encoding &
decoding.

All of the above libraries are located in `common/extlib/<libname>/`.

Our own written libraries are in `common/<libname>`.

SDL: https://libsdl.org/download-2.0.php (Required: 2.0.0 or greater)

SDL\_Image: https://libsdl.org/projects/SDL_image/ (Required: 2.0.0 or greater)

SDL\_Mixer: http://libsdl.org/projects/SDL_mixer/ (Required: 2.0.0 or greater)

SDL\_TTF: http://libsdl.org/projects/SDL_ttf/ (Required: 2.0.0 or greater)

Building
========

There are build instructions for [Mac](https://github.com/TeamLe-Shop/zordzman-plus-plus#mac-os-x)
and [Linux](https://github.com/TeamLe-Shop/zordzman-plus-plus#linux) below.

### Mac OS X ###

Requires XCode, at least 4.2. Downloadable from the App Store.
Requires CMake, at least 3.0.0.

Download SDL2 and the additional SDL2 libraries (image, mixer etc.).

Open SDL2.dmg and additional SDL2 library images.
Move `SDL2.framework` into `/Library/Frameworks`, along with the additional
`.framework` files found in the other images.
Inside the SDL_x (e.g. SDL\_image) framework directories,
move the C header file found in `Headers/` into a new directory called "SDL2"
(inside the "Headers" directory).

Navigate to where XCode.app is, and enter the Package Contents (often accessed
by right clicking on XCode.app in Finder, and selecting "Show Package Contents".)
Then navigate to `Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.x.sdk/Developer/System/Library/Frameworks` and copy `OpenGL.framework` into
`/Library/Frameworks`.

Create a new CMake variable (easily done using the CMake GUI and clicking the
"Add Entry" button) called "OSX", type boolean and set it to true.
When building the project, this will make sure CMake finds the SDL framework
directories and header files. This will also build with compile options
`-stdlib=libc++` and `-std=c++11`. If you do not want these compile options you
can manually set them yourself.

Then navigate back to the zordzman-plus-plus folder, and proceed to do the CMake instructions for Linux.

### Linux ###

Requires CMake, at least 3.0.0.

If you don't know how to install SDL on Linux, check out this tutorial:
[LazyFoo](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php)

Use [CMake](http://cmake.org/) to build.
If you don't know how to use CMake, do the following:

Assuming you are in the zordzman-plus-plus directory, do these steps:
```
mkdir ../build-zordzman-plus-plus
cd ../build-zordzman-plus-plus
cmake ../zordzman-plus-plus
make
```

The produced binary will be in `build-zordzman-plus-plus`.
It is recommended that the binary is inside the `zordzman-plus-plus` directory
as the resources and configuration files will be in that directory.

You can also optionally perform `cmake .` inside `zordzman-plus-plus`, but this will clutter the directory with build files.

### Windows ###

We are currently working towards windows support. See
[PR #24](https://github.com/TeamLe-Shop/zordzman-plus-plus/issues/24)
and https://github.com/Holiverh/zordzman-plus-plus


Contributing
============

Anyone is welcome to contribute!
Just make sure to read [CONTRIBUTING.md](CONTRIBUTING.md) before doing so!

We'll try our best to make collaborating on Zordzman with any OS a good experience!
