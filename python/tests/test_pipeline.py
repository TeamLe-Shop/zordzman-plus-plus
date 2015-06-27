import types

import pytest

import zm.pipeline


class Pipeline(zm.pipeline.Pipeline):
    """A pipeline to be used by tests."""


class TestPipelineMeta:

    def test_subscribe(self):
        subscriber = pytest.Mock()
        assert Pipeline.subscribe(
            "test", unpack=True, silence=True)(subscriber) is subscriber
        subscriber_specs = subscriber.__pipeline_subscriptions__[Pipeline]
        assert len(subscriber_specs) == 1
        assert subscriber_specs[0].type == "test"
        assert subscriber_specs[0].function is subscriber
        assert subscriber_specs[0].unpack is True
        assert subscriber_specs[0].silence is True

    def test_subscribe_default_unpack(self):
        subscriber = pytest.Mock()
        assert Pipeline.subscribe(
            "test", silence=True)(subscriber) is subscriber
        subscriber_specs = subscriber.__pipeline_subscriptions__[Pipeline]
        assert len(subscriber_specs) == 1
        assert subscriber_specs[0].type == "test"
        assert subscriber_specs[0].function is subscriber
        assert subscriber_specs[0].unpack is False

    def test_subscribe_default_silence(self):
        subscriber = pytest.Mock()
        assert Pipeline.subscribe(
            "test", unpack=True)(subscriber) is subscriber
        subscriber_specs = subscriber.__pipeline_subscriptions__[Pipeline]
        assert len(subscriber_specs) == 1
        assert subscriber_specs[0].type == "test"
        assert subscriber_specs[0].function is subscriber
        assert subscriber_specs[0].silence is False

    def test_scan_object(self, monkeypatch):
        monkeypatch.setattr(Pipeline,
                            "__init__", pytest.Mock(return_value=None))
        obj = types.SimpleNamespace(subscriber=pytest.Mock)
        Pipeline.subscribe("test")(obj.subscriber)
        assert isinstance(Pipeline.scan(obj), Pipeline)
        assert len(Pipeline.__init__.call_args[0][0]) == 1
        subscriber = Pipeline.__init__.call_args[0][0][0]
        assert subscriber.type == "test"
        assert subscriber._function is obj.subscriber
        assert subscriber._unpack is False
        assert subscriber._silence is False
