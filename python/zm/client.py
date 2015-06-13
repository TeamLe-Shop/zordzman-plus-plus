import collections
import json
import logging
import socket
import threading

import formencode

import zm.pipeline


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


class ClientError(Exception):
    """Base exception raised for all errors within the client."""


class InvalidMessageError(ClientError):
    """Raised when the client recieved a malformed message."""


@zm.pipeline.Egress.register("zm:client:connect")
class ConnectMessage(formencode.Schema):
    """Client connection message."""

    host = formencode.validators.IPAddress()
    port = formencode.validators.Int(min=0, max=65535)


@zm.pipeline.Egress.register("zm:client:disconnect")
class DisconnectMessage(formencode.Schema):
    """Client disconnection message."""


@zm.pipeline.Ingress.register("entity.state")
class EntityStateMessage(formencode.Schema):

    id = formencode.validators.Int()
    component = formencode.validators.String()
    field = formencode.validators.String()
    value = formencode.Pipe(validators=[
        formencode.Any(validators=[
            formencode.validators.ConfirmType(type=list),
            formencode.validators.ConfirmType(type=dict),
            formencode.validators.ConfirmType(type=int),
            formencode.validators.ConfirmType(type=float),
            formencode.validators.ConfirmType(type=str),
        ]),
        formencode.validators.ConfirmType(type=object),
    ])

@zm.pipeline.Egress.register("chat.message")
@zm.pipeline.Ingress.register("chat.message")
class ChatMessage(formencode.Schema):

    message = formencode.validators.String()


class Client(threading.Thread):
    """The Zordzman network client.

    This is a client which runs in its own thread that handles the
    socket-level communication between Zordzman and a remote server. It
    provides the API for the C++ wrapper (``net::Client``) which lets messages
    be recieved and sent from C++.

    Each client maintains two message 'pipelines'. These pipelines are
    responsible for validating messages that are passed into them. The ingress
    pipeline holds messages which are said to be moving 'towards' the C++
    layer. E.g. messages from the server.

    The egress pipeline is for messages in the opposite direciton, as in, away
    from the C++ layer towards the server.

    Each message has two components: a type and an entity. The type, primarily
    identified by its name as a string describes the structure of the entity.
    The entity is the main content of the message and are exposed as plain
    Python dictionaries.

    The ingress and egress pipelines will only accept messages which are of
    a type that has been registerd with each respective pipeline. Hence, if
    a message is received which the client doesn't know about it will be
    discarded and will never reach the C++ layer. The same applies in the
    opposite direction -- the C++ layer cannot attempt to send a message
    the pipeline doesn't know how to process.

    As well as acting as convenient containers for messages, pipelins also
    allow Python functions to subscribe to certain message types. These
    subscribers are called when a message is recieved of the matching type.

    Furthermore, message subscribers can 'silence' a message which prevents
    them moving through the pipeline any further. This means pipelines can
    be used for communication between the C++ and Python levels without the
    worry of messages leaking out to the remote server.

    In fact, this is how the C++ level tells the client where to connect to,
    by sending it a ``zm:connect`` message. And it's the same for
    disconnecting where a ``zm:disconnect`` message is sent instead.
    """

    MAGIC_NUMBER = b"\xCA\xC3\x55\x01"

    def __init__(self):
        """Initialise the client.

        This does not actually start the client. :meth:`start` must be called
        separately.
        """
        super().__init__(name=self.__class__.__name__ + "Thread")
        self._egress = zm.pipeline.Egress.scan(__package__, self)
        self._ingress = zm.pipeline.Ingress.scan(__package__, self)
        self._socket = None
        self._peer_name = None
        self._ingress_buffer = b""
        self._egress_buffer = b""

    @zm.pipeline.Egress.subscribe("zm:client:disconnect", silence=True)
    def disconnect(self, entity=None):
        """Disconnect from a server.

        This closes the client socket and clears all the message pipelines
        and buffers.

        If there's no current connection then this is a no-op.
        """
        if self._socket:
            self._socket.close()
            self._ingress_buffer = b""
            self._egress_buffer = b""
            self._ingress.clear()
            self._egress.clear()
            log.info("Disconnected from %s:%i" % self._peer_name)

    @zm.pipeline.Egress.subscribe("zm:client:connect",
                                  unpack=True, silence=True)
    def connect(self, host, port):
        """Establish a connection with a server.

        If the client is already connected to a server then it will disconnect
        first and then connect to the new one.
        """
        if self._socket:
            self.disconnect()
        log.info("Connecting to %s:%i", host, port)
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect((host, port))
        self._peer_name = self._socket.getpeername()
        self._socket.sendall(self.MAGIC_NUMBER)

    def retrieve(self):
        raise NotImplementedError

    def send(self, type_name, entity):
        """Send a message to the server.

        The message is passed through the egress pipeline and hence may be
        silenced, preventing it from ever reaching the server.

        :param type_name: the name of the type of the message.
        :param entity: the message entity dictionary.
        """
        self._egress.push(type_name, entity)

    def _parse_messages(self):
        """Parse messages from the ingress buffer.

        This provides an iterator that parses the raw, space-separated JSON
        objects from the ingress buffer. The iterator consumes the buffer as
        it is iterated over.
        """
        messages = []
        i = 0
        while i < len(self._ingress_buffer):
            part = self._ingress_buffer[:i].decode()
            try:
                message = json.loads(part)
            except ValueError:
                pass
            else:
                self._ingress_buffer = self._ingress_buffer[i:]
                i = 0
                yield message
            i += 1

    def _enqueue_message(self, message):
        """Validate a message and push it into the ingress pipeline.

        This takes a JSON-decoded message, and will attempt to add it to the
        ingress pipeline. The given message should be a dictionary with
        ``type`` and ``entity`` keys. The ``type`` should correspond to a
        :class:`zm.pipeline.MessageType` registered with the pipeline.

        :raises InvalidMessageError: if the message is not a dictionary, the
            message type is not accepted by the ingress pipeline or the
            message entity fails to validate as the claimed message type.
        """
        if not isinstance(message, dict):
            raise InvalidMessageError(
                "Message top-level object should be a dictionary")
        try:
            self._ingress.push(
                message.get("type"), message.get("entity"))
        except zm.pipeline.PipelineError as exc:
            raise InvalidMessageError(exc) from exc

    def _read(self):
        """Read messages from the socket.

        This will read data from the connected socket and add it to the
        ingress buffer. An attempt is then made to parse the JSON-encoded
        messages from the buffer. If there are any messages in the buffer
        then they will be enqueued in the ingress pipeline.

        If a message is invalid for any reason then it is discarded and a
        warning is logged.

        If there is currently no connection associated with the client then
        this is a no-op.
        """
        if self._socket:
            recv = self._socket.recv(4096)
            if not recv:
                self.disconnect()
            self._ingress_buffer += recv
            for message in self._parse_messages():
                try:
                    self._enqueue_message(message)
                except InvalidMessageError as exc:
                    log.warning("Invalid message: %s", exc)

    def _write(self):
        """Write message to the socket.

        This pulls messages from the egress pipeline and encodes them as
        JSON and attempts to send them over the currently connected socket.
        If not all of the messages can be sent they are buffered in the egress
        buffer.

        Each JSON message is a JSON object (a dictionary) with two keys:

         * ``type``: the message type name as a string.
         * ``entity``: a dictionary reprenting the message contents.

        Each message is terminated by a single SPACE character and is UTF-8
        encoded.
        """
        if self._socket:
            for message in self._egress:
                json_message = json.dumps({"type": message.type.name,
                                           "entity": message.entity}) + " "
                self._egress_buffer += json_message.encode()
            sent = self._socket.send(self._egress_buffer)
            self._egress_buffer = self._egress_buffer[sent:]


    def run(self):
        """The client mainloop.

        This continuously reads and writes messages to and from the socket
        (if connected) and pumps messages through the ingress and egress
        pipelines.
        """
        log.info("Client running")
        while True:
            self._egress.pump()
            self._read()
            self._write()
            self._ingress.pump()
        log.info("Client stopped")
