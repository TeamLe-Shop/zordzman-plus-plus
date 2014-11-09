
import json
import socket
import struct


MAGIC = b"\xCA\xC3\x55\x01"


def encode_message(type_, entity):
    return json.dumps({"type": str(type_),
                       "entity": entity}).encode("utf-8") + b" "


if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect(("localhost", 4544))
        sock.sendall(MAGIC)
        sock.sendall(encode_message("map.request",
                                    "a9531495903a650967c81781c1694d05"))
        if input("close socket?"):
            pass
        print("recv:", sock.recv(8192))
    finally:
        sock.close()
