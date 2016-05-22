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

Requirements: Python3, [Pip](https://pip.pypa.io/en/stable/installing/),
[CMake](https://cmake.org/download/)

After cloning the repository, enter the root directory of the project and install formencode by executing the following:
```sh
pip install --user formencode
```

After formencode has been successfully installed, install zordzman's python packages by executing the following:

```sh
pip install --user -e .
```

If installation of these packages was successful, you can now build zordzman. It is, however, recommended that you create an external `build-zordzman-plus-plus` directory.

```sh
mkdir ../build-zordzman-plus-plus
cd ../build-zordzman-plus-plus
cmake zordzman-plus-plus
make
````

To run the game however, a `resources.tar` resource package is required, which
is created at build time. It will be found inside
`build-zordzman-plus-plus/resources` assuming you followed the steps above.

```sh
cp resources/resources.tar ../zordzman-plus-plus/resources.tar
```

The above line will copy the resource package into the correct location.

### Windows ###

We are currently working towards windows support. See
[PR #24](https://github.com/TeamLe-Shop/zordzman-plus-plus/issues/24)
and https://github.com/Holiverh/zordzman-plus-plus


Contributing
============

Anyone is welcome to contribute!
Just make sure to read [CONTRIBUTING.md](CONTRIBUTING.md) before doing so!
