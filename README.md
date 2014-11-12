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

It mainly uses SDL and other SDL libraries.

Note: SDLNet is no longer being used, and our project will be using system sockets instead.

It uses [json11](https://github.com/dropbox/json11) for parsing JSON,
[cppformat](http://cppformat.github.io/) for formatting, and
[hash-library](http://create.stephan-brumme.com/hash-library/) for, guess what,
hashing. (HOLY SIT)

All three libraries are located in `common/extlib/libname/`.

Our own written libs are in `common/libname`.

SDL: https://libsdl.org/download-2.0.php (Required: 2.0.0 or greater)

SDL\_Image: https://libsdl.org/projects/SDL_image/ (Required: 2.0.0 or greater)

SDL\_Mixer: http://www.libsdl.org/projects/SDL_mixer/ (Required: 2.0.0 or greater)

Building
========

There are build instructions for [Mac](https://github.com/TeamLe-Shop/zordzman-plus-plus#mac-os-x)
and [Linux](https://github.com/TeamLe-Shop/zordzman-plus-plus#linux) below.

### Mac OS X ###

REQUIRES XCODE (atleast 4.2, free), download from the App store

Download SDL2, SDL2_image and SDL2\_mixer.

Then, after you've opened your `SDL2.dmg`, copy the `SDL2.framework` and
move it into `/Library/Frameworks` directory. You can find this by navigating to Macintosh HD (usually by double clicking the icon on the desktop) and going to `/Library/Frameworks`.
Then, do the same with `SDL2_image.dmg` and `SDL2_mixer.dmg`.

The next part requires [XCode](https://developer.apple.com/xcode/downloads/), and what you need to do is find your XCode.app, right click, select "Show Package Contents" and
you'll be brought to the folder "Contents".
Navigate to
`Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.x.sdk/Developer/System/Library/Frameworks`
and copy `OpenGL.framework` into `/Library/Frameworks` (back where you put the SDL stuff).

Then navigate back to the zordzman-plus-plus folder, and proceed to do the CMake instructions for Linux. They are almost the exact same.


### Linux ###

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


Contributing
============

Anyone is welcome to contribute!
Just make sure to read [CONTRIBUTING.md](CONTRIBUTING.md) before doing so!

We'll try our best to make collaborating on Zordzman with any OS a good experience!
