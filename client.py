
import json
import socket
import struct


MAGIC = b"\xCA\xC3\x55\x01"


def encode_message(type_, entity):
    payload = json.dumps({"type": str(type_),
                          "entity": entity}).encode("utf-8") + b" "
    print(repr(payload))
    return payload


if __name__ == "__main__":
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    udp_sock.bind(("", 0))
    try:
        sock.connect(("localhost", 4544))
        sock.sendall(MAGIC)
        sock.sendall(encode_message("map.request", None))
        sock.sendall(encode_message("net.udp", udp_sock.getsockname()[1]))
        if input("close socket?"):
            pass
        print("recv:", sock.recv(8192))
    finally:
        sock.close()
