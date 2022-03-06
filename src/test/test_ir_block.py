#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from unittest.mock import MagicMock
from hugo_test_case import TestCase #must be called before another imports from devel or micropython - redefine import
from ___blocks.___ir_block import IrBlock
from ___remote_control.___remote_key import RemoteKey
from ___remote_control.___remote_keyboard_base import RemoteKeyboardBase

class TestingRemote(RemoteKeyboardBase):
  def get_address(self):
    return 0xFF00

  def find_name_by_scan_code(self, scan_code):
    return "bla"

class TestIrBlock(TestCase):
  def setUp(self):
    self.ir_block = IrBlock(measurement_period=0)
    self.ir_block.add_remote(TestingRemote())
    # pylint: disable=protected-access
    self.ir_block._tiny_read = MagicMock()

  def _value_remote_get_test(self, address):
    # pylint: disable=protected-access
    self.ir_block._tiny_read.side_effect = [b"1", address.to_bytes(2, "little") + b"\x01\x00"]
    key: RemoteKey = self.ir_block.value.get(force=True)
    self.assertEqual(key.address, 0xFF00)
    self.assertEqual(key.scan_code, 0x01)
    self.assertEqual(key.name, "bla")

  def test_value_remote_get_exact_address(self):
    self._value_remote_get_test(0xFF00)

  def test_value_remote_get_exact_address_near_address(self):
    self._value_remote_get_test(0xFF00 + 0b11)

  def test_value_remote_get_exact_address_gar_address(self):
    # pylint: disable=protected-access
    self.ir_block._tiny_read.side_effect = [b"1", (0xFF00 + 0b111).to_bytes(2, "little") + b"\x01\x00"]
    key = self.ir_block.value.get(force=True)
    self.assertEqual(key, RemoteKey.get_default())
