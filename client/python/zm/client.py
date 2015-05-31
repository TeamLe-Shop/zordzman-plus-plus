import json
import logging
import socket
import threading


log = logging.getLogger(__name__)


class ZMClient(threading.Thread):

    MAGIC_NUMBER = b"\xCA\xC3\x55\x01"

    def __init__(self):
        super().__init__(name=self.__class__.__name__ + "Thread")
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._buffer = b""

    def _connect(self, host, port):
        log.info("Connecting to %s:%i", host, port)
        self._socket.connect((host, port))
        self._socket.sendall(self.MAGIC_NUMBER)

    def _parse_messages(self):
        messages = []
        i = 0
        while i < len(self._buffer):
            part = self._buffer[:i].decode()
            try:
                message = json.loads(part)
            except ValueError:
                pass
            else:
                messages.append(message)
                self._buffer = self._buffer[i:]
            i += 1
        return messages

    def run(self):
        self._connect("127.0.0.1", 9001)
        while True:
            recv = self._socket.recv(4096)
            if not recv:
                log.info("Disconnected from server")
                break
            self._buffer += recv
            messages = self._parse_messages()
            if messages:
                log.debug("%i new messages; %i bytes remain "
                          "in the buffer", len(messages), len(self._buffer))
            log.debug(messages)
        log.info("Client exiting")
