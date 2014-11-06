
import json
import socket
import struct


MAGIC = b"\xCA\xC3\x55\x00"


def encode_message(type_, entity):
    return json.dumps({"type": str(type_),
                       "entity": entity}).encode("utf-8")


if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect(("localhost", 4544))
        sock.sendall(MAGIC)
        sock.sendall(encode_message("hello", "world"))
        if input("close socket?"):
            pass
    finally:
        sock.close()
