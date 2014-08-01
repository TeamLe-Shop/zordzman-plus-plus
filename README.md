zordzman-plus-plus
==================

The best game that ever existed. Period.

Zordzman-plus-plus (Zordzman++) will be a remake of
the original Zordzman created by JavaCakess in, well, Java.
This remake shall be in C++, and will be developed by a team
rather than one person.

Libraries
=========

It uses SDL, SDL_Image and OpenGL for about everything right now.
Feel free to... Tag Along!


SDL: https://www.libsdl.org/download-2.0.php (Recommended: 2.0.0)

SDL-Image: https://www.libsdl.org/projects/SDL_image/ (Recommended: 2.0.0)

Mac OS X
========

For OSX, rename OSX_Makefile to "Makefile" and build with Make.

Then, after you've opened your SDL2 .dmg, copy the SDL2.framework and
move it into /Library/Frameworks directory. You can find this by navigating to Macintosh HD (usually by double clicking the icon on the desktop) and going to /Library/Frameworks.
Then, do the same with the SDL2_image dmg.

The next part requires XCode, and what you need to do is find your XCode.app, right click, select "Show Package Contents" and 
you'll be brought to the folder "Contents".
Navigate to Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.x.sdk/Developer/System/Library/Frameworks
and copy OpenGL.framework (It's a folder, remember!) into /Library/Frameworks (back where you put the SDL stuff).

That's about it. It's can be a little tedious, but once that's all set up you're ready to move on!

Then navigate back to the zordzman-plus-plus folder where the OSX_Makefile is (now renamed Makefile) and run "make".
Then, run "./zordzman" and the game should be brought up!