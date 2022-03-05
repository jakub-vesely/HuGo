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

    if name in ("machine", "bluetooth"):
      print(f"{name} mocked by MagicMoc")
      return cls.module_mock()

    if name in ("micropython",):
      print(f"{name} mocked by file")
      return cls.orig_import("test." + name + "_mock", *args)

    if ("___") in name :
      print(f"__init__ {name} redirected to devel")
      return cls.orig_import("devel." + name, *args)

    return cls.orig_import(name, *args)

builtins.__import__ = TestCase.import_mock