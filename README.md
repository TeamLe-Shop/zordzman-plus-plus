zordzman-plus-plus
==================
![](http://servers.tf:8080/buildStatus/icon?job=zordzman-plus-plus)

The best game that ever existed, if it weren't for JavaCakess' cancerous changes.

Zordzman-plus-plus (Zordzman++) will be a remake of
the original Zordzman created by JavaCakess in, well, Java.
This remake shall be in C++, and will be developed by a team
rather than one person.

Dependencies
=========

It uses SDL, SDL_Image and OpenGL for about everything right now.

SDL: https://www.libsdl.org/download-2.0.php (Required: 2.0.0 or greater)

SDL-Image: https://www.libsdl.org/projects/SDL_image/ (Required: 2.0.0 or greater)

Building
========

There are build instructions for [Mac](https://github.com/TeamLe-Shop/zordzman-plus-plus#mac-os-x)
and [Linux](https://github.com/TeamLe-Shop/zordzman-plus-plus#linux) below.

### Mac OS X ###

Download SDL2 and SDL2_image.

Then, after you've opened your SDL2 .dmg, copy the SDL2.framework and
move it into /Library/Frameworks directory. You can find this by navigating to Macintosh HD (usually by double clicking the icon on the desktop) and going to /Library/Frameworks.
Then, do the same with the SDL2_image dmg.

The next part requires [XCode](https://developer.apple.com/xcode/downloads/), and what you need to do is find your XCode.app, right click, select "Show Package Contents" and 
you'll be brought to the folder "Contents".
Navigate to Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.x.sdk/Developer/System/Library/Frameworks
and copy OpenGL.framework (It's a folder, remember!) into /Library/Frameworks (back where you put the SDL stuff).

That's about it. It's can be a little tedious, but once that's all set up you're ready to move on!

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

Anyone is welcome to contribute! (Apart from gatsan lil)
Just make sure to read [CONTRIBUTING.md](CONTRIBUTING.md) before doing so!

We'll try our best to make collaborating on Zordzman with any OS a good experience!
