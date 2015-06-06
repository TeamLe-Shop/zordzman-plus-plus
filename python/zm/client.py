import collections
import json
import logging
import socket
import threading

import formencode


log = logging.getLogger(__name__)


Message = collections.namedtuple(
    "Message",
    (
        "type",
        "entity",
    ),
)


class EntityStateSchema(formencode.Schema):

    id = formencode.validators.Int()
    component = formencode.validators.String()
    field = formencode.validators.String()
    value = formencode.Any(validators=[
        formencode.validators.ConfirmType(type=list),
        formencode.validators.ConfirmType(type=dict),
        formencode.validators.ConfirmType(type=int),
        formencode.validators.ConfirmType(type=float),
        formencode.validators.ConfirmType(type=str),
    ])


MESSAGE_TYPES = {
    "entity.state": EntityStateSchema,
    # "player.id": formencode.validators.Int(),
}


def validate_entity(message, state, validator):
    type_ = message["type"]
    entity = message["entity"]
    try:
        message["entity"] = MESSAGE_TYPES[type_].to_python(entity)
    except formencode.Invalid as invalid:
        raise formencode.Invalid(
            "entity: " + invalid.msg,
            message,
            state,
            error_dict={"entity": invalid},
        )


class MessageSchema(formencode.Schema):

    type = formencode.validators.OneOf(list(MESSAGE_TYPES.keys()))
    entity = formencode.Any(validators=[
        formencode.validators.ConfirmType(type=list),
        formencode.validators.ConfirmType(type=dict),
        formencode.validators.ConfirmType(type=int),
        formencode.validators.ConfirmType(type=float),
        formencode.validators.ConfirmType(type=str),
    ])
    chained_validators = [
        formencode.schema.SimpleFormValidator(validate_entity)]


class ZMClient(threading.Thread):

    MAGIC_NUMBER = b"\xCA\xC3\x55\x01"

    def __init__(self):
        super().__init__(name=self.__class__.__name__ + "Thread")
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._buffer = b""
        self.messages = []
        self.in_messages = []

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

    def _validate_messages(self, messages):
        validated = []
        for message in messages:
            try:
                message = MessageSchema.to_python(message)
            except formencode.Invalid as exc:
                log.warning("Invalid message: %s", exc)
            else:
                validated.append(Message(message["type"], message["entity"]))
        return validated

    def run(self):
        self._connect("127.0.0.1", 9001)
        while True:
            log.debug(self.in_messages)
            recv = self._socket.recv(4096)
            if not recv:
                log.info("Disconnected from server")
                break
            self._buffer += recv
            messages = self._parse_messages()
            if messages:
                log.debug("%i new messages; %i bytes remain "
                          "in the buffer", len(messages), len(self._buffer))
            self.messages.extend(self._validate_messages(messages))
            # raise Exception
        log.info("Client exiting")
