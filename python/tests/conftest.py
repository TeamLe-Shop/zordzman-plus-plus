import unittest.mock

import pytest


def pytest_namespace():
    return {
        "Mock": unittest.mock.Mock,
        "MagicMock": unittest.mock.MagicMock,
    }
