zordzman-plus-plus
==================

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

The networking layer as well as some build scripts use Python, and so Python 3
is required.

### Mac OS X ###

Will be written soon.

### Linux ###

Will be written soon.

### Windows ###

We are currently working towards windows support. See
[PR #24](https://github.com/TeamLe-Shop/zordzman-plus-plus/issues/24)
and https://github.com/Holiverh/zordzman-plus-plus


Contributing
============

Anyone is welcome to contribute!
Just make sure to read [CONTRIBUTING.md](CONTRIBUTING.md) before doing so!
