import logging
import threading


log = logging.getLogger(__name__)


class ZMClient(threading.Thread):

    def __init__(self):
        super().__init__(name=self.__class__.__name__ + "Thread")

    def run(self):
        log.info("Client running")
        log.info("Client exiting")
