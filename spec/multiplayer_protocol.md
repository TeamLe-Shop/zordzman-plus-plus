Zordzman Multiplayer Protocol
=============================

The first thing the client and server have to do is "handshake". They perform this over
a reliable connection (TCP).
The handshake is to make sure they're using the same protocol.
If the client is using the wrong protocol, the connection is immediately terminated.

Everything (apart from the handshake) sent over TCP will be in JSON. An example:

```javascript
{
    "type": "credentials",
    "entity": {
        "username": "octozordz"
    }
}
```

Each JSON message will have a "type" field that specifies the type of message.
The above example showed a "credentials" message, for when the player joins.
The additional fields specific to that message is inside "entity".