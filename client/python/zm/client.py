import logging
import socket
import threading


log = logging.getLogger(__name__)


class ZMClient(threading.Thread):

    MAGIC_NUMBER = b"\xCA\xC3\x55\x01"

    def __init__(self):
        super().__init__(name=self.__class__.__name__ + "Thread")
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def _connect(self, host, port):
        log.info("Connecting to %s:%i", host, port)
        self._socket.connect((host, port))
        self._socket.sendall(self.MAGIC_NUMBER)

    def run(self):
        self._connect("127.0.0.1", 9001)
        while True:
            break
        log.info("Client exiting")
