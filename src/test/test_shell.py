from hugo_test_case import TestCase #must be called before another imports from devel or micropython - redefine import
from ___shell import Shell, _cmd_get_next_file_info

class TestShell(TestCase):

  def setUp(self):
    self.shell = Shell()
    self.shell.path = ["./test/devel"] #to be searching started in prepared mocked directory

  def test_command_request__get_next_file_info(self):
    value = self.shell.command_request([_cmd_get_next_file_info])
    self.assertEqual(value, b'\xc1e\x95\x97\xb2$\xe0NeAH\ns\xd6G\x04\xfdU\x91\xc4./test/devel/boot.py')

    value = self.shell.command_request([_cmd_get_next_file_info])
    self.assertEqual(value, b'hS\xa8\xe9\xedR\xa2\x0c\xcck\xd7\x10\xfb\xe0j\x86\x02`K2./test/devel/remote_control/remote_file.py')

    value = self.shell.command_request([_cmd_get_next_file_info])
    self.assertEqual(value, b'\x00')

