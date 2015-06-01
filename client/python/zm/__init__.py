import logging
import sys
import time

import zm.client


def setup_logging(level):
    log_format = "{asctime} {levelname:8} {threadName} {name} {message}"
    log = logging.getLogger("")
    handler = logging.StreamHandler(stream=sys.stderr)
    format_ = logging.Formatter(fmt=log_format, style="{")
    format_.converter = time.gmtime
    handler.setFormatter(format_)
    log.setLevel(level)
    log.addHandler(handler)
    return logging.getLogger("zm")


def main():
    log = setup_logging(logging.INFO)
    log.info("Python layer running, log level = %i", logging.DEBUG)
    client = zm.client.ZMClient()
    client.start()
    return client
