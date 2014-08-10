Zordzman Multiplayer Protocol
=============================

The first byte of the packet represents the type.
The rest of the content is split by a comma (,) and packed into some form of an array.

Types:

Byte | Packet Type | Parameters
-----|-------------|-----------
0x00 | Login       | Username
0x01 | Move        | ( Dunno yet )
