"""Message processing pipelines."""


import collections
import copy
import functools
import inspect
import logging


log = logging.getLogger(__name__)


class PipelineError(Exception):
    """Raised by pipelines."""


class EmptyPipelineError(PipelineError):
    """Raised when performing certain operations on an empty pipeline."""


class Silence(Exception):
    """Raised to silence messages in a pipeline."""


SubscriberSpec = collections.namedtuple(
    "SubscriberSpec",
    (
        "type",
        "function",
        "unpack",
        "silence",
    )
)


class Subscriber:
    """Convenience wrapper for message subscribers."""

    def __init__(self, type_, function, unpack, silence):
        self.type = type_
        self._function = function
        self._unpack = unpack
        self._silence = False

    def __repr__(self):
        return ("<{0.__class__.__name__} "
                "{0._function.__qualname__} for {0.type!r}>".format(self))

    def __call__(self, entity):
        if self._unpack:
            self._function(**entity)
        else:
            self._function(entity)
        raise Silence


class PipelineMeta(type):
    """Metaclass for message pipelines.

    This metaclass provides the :meth:`subscribe` decorator for registering
    subscribers with a message pipeline. Each class with this metaclass has
    its own isolated set of subscibers.

    Instances of the pipeline should be created using :meth:`scan` which will
    search for all subscribes that were registered with :meth:`subscribe`.
    """

    _no_entity = object()
    marker = "__pipeline_subscriptions__"

    def __new__(meta, name, bases, attrs):
        """Construct a new message pipeline class.

        This adds a ``types`` list to the class which contains all of the
        :class:`MessageType`s that have been registered with the pipeline.
        """
        attrs["types"] = []
        return super().__new__(meta, name, bases, attrs)

    def subscribe(cls, type_, *, unpack=False, silence=False):
        """Register a message subscriber.

        This is a decorator which will make the wrapped function be picked up
        by later called to :meth:`scan`.
        """

        def decorator(function):
            subscriptions = getattr(function, cls.marker, {})
            if cls not in subscriptions:
                subscriptions[cls] = []
            subscriptions[cls].append(
                SubscriberSpec(type_, function, unpack, silence))
            setattr(function, cls.marker, subscriptions)
            return function

        return decorator

    def scan(cls, *objects):
        """Scan packages, modules and objects for subscribers.

        This will construct a new pipeline, scanning the given objects for
        subscribers. If passed a string it will be interpretted as a module
        name to import and then scan. Plain objects can also be given instead
        of module names.

        :params objects: objects or module names as strings to be scanned.

        :returns: a new pipeline instance.
        """
        subscribers = []
        for obj in objects:
            if isinstance(obj, str):
                obj = __import__(obj)
            for _, member in inspect.getmembers(obj):
                subscriptions = getattr(member, cls.marker, {})
                for subscription in subscriptions.get(cls, []):
                    subscribers.append(Subscriber(subscription.type,
                                                  member,
                                                  subscription.unpack,
                                                  subscription.silence))
        return cls(subscribers)

    def register(cls, type_name):

        def decorator(schema):
            cls.types.append(MessageType(type_name, schema))
            return schema

        return decorator


Message = collections.namedtuple(
    "Message",
    (
        "type",
        "entity",
    )
)


class MessageType:

    def __init__(self, name, schema):
        self.name = name
        self._schema = schema

    def __repr__(self):
        return "<{0.__class__.__name__} {0.name!r}>".format(self)

    def validate(self, entity):
        return self._schema.to_python(entity)


class Pipeline(metaclass=PipelineMeta):

    def __init__(self, subscribers):
        self._message_types = {type_.name: type_ for type_ in self.types}
        self._subscribers = {}
        for subscriber in subscribers:
            if subscriber.type not in self._subscribers:
                self._subscribers[subscriber.type] = []
            self._subscribers[subscriber.type].append(subscriber)
        self._pending = collections.deque()
        self._complete = collections.deque()

    def __iter__(self):
        """Iterator of complete messages.

        This consumes the completed messages.
        """
        while True:
            try:
                yield self.pop()
            except EmptyPipelineError:
                break

    def clear(self):
        """Clear all messages in the pipeline.

        This clears all messages, regardless of whether they are pending or
        completed.
        """
        seld._pending.clear()
        self._complete.clear()

    def pump(self):
        for message in self._pending:
            for subscriber in self._subscribers.get(message.type, []):
                entity = copy.deepcopy(message.entity)
                try:
                    subscriber(entity)
                except Silence:
                    log.debug("%s silenced by %s", message, subscriber)
                    break
                except Exception:
                    log.exception("Exception in %s whilst "
                                  "processing %s", subscriber, message)
                    continue
            else:
                self._complete.append(message)
        self._pending.clear()

    def push(self, type_or_name, entity):
        if isinstance(type_or_name, MessageType):
            type_ = type_or_name
        else:
            if type_or_name not in self._message_types:
                raise PipelineError("Pipeline does not accept messages "
                                    "of type {!r}".format(type_or_name))
            type_ = self._message_types[type_or_name]
        entity = type_.validate(entity)
        self._pending.append(Message(type_, entity))

    def pop(self):
        try:
            return self._complete.popleft()
        except IndexError as exc:
            raise EmptyPipelineError from exc


class Ingress(Pipeline):
    """The ingress message pipeline.

    For messages flowing 'towards' the C++ layer.
    """


class Egress(Pipeline):
    """The egress message pipeline.

    For messages flowing 'away from' the C++ layer.
    """
