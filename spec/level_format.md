Zordzman Level Format
=====================

The first two bytes represent width and height. The next two are the player spawn point X and Y position.
It will be multiplied by 32 (tile size) in the game, so `0F 04` would make the player spawn at 480, 128.

After that is a series of bytes, each representing a tile id.

This may change quickly as Zordzman is still in early development.
