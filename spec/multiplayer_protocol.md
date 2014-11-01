Zordzman Multiplayer Protocol
=============================

The first thing the client and server have to do is "handshake". They perform this over
a reliable connection (TCP).
The handshake is to make sure they're using the same protocol.
If the client is using the wrong protocol, the connection is immediately terminated.

Everything (apart from the handshake) sent over TCP will be in JSON. An example:

```javascript
{
    "type": "map-hash",
    "entity": {
        "hash": "a9531495903a650967c81781c1694d05"
    }
}
```

Each JSON message will have a "type" field that specifies the type of message.
The above example shows a "map-hash" message, for when the server asks the client
if it has a certain map.
The additional fields specific to that message is inside "entity".

After the handshake, the server sends over a hash of the current map to the client,
who then checks if they have the map or not by running through their directory of maps
and seeing if any of their filenames match the hash.

The client must respond whether or not it has this map. If it does,
the client can just proceed to joining the game, if not the server sends the map to the client
encoded in Base 64.