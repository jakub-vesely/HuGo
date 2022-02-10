import unittest
from unittest.mock import MagicMock
import builtins

class TestCase(unittest.TestCase):
  orig_import = __import__
  module_mock = MagicMock()

  @classmethod
  def import_mock(cls, name, *args):
    if name == "uasyncio":
      return cls.orig_import("asyncio")

    if name in ('machine',):
      print(f"{name} mocked")
      return cls.module_mock()

    if name.startswith("___"):
      return cls.orig_import("src.devel." + name, *args)

    return cls.orig_import(name, *args)

builtins.__import__ = TestCase.import_mock


