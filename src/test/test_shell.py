#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from hugo_test_case import TestCase #must be called before another imports from devel or micropython - redefine import
from ___basal.___shell import Shell, _cmd_shell_get_next_file_info

class TestShell(TestCase):

  def setUp(self):
    self.shell = Shell()
    self.shell.path = ["./test/devel"] #to be searching started in prepared mocked directory

  def test_command_request__get_next_file_info(self):
    value = self.shell.command_request([_cmd_shell_get_next_file_info])
    self.assertEqual(value, b'\xb3\x8au\xdds\xf9\x99\xe0\xf2\xe9\x92\xb4t\x14\x8e<\x18\xa4\xd5\x89./test/devel/boot.py')

    value = self.shell.command_request([_cmd_shell_get_next_file_info])
    self.assertEqual(value, b'hS\xa8\xe9\xedR\xa2\x0c\xcck\xd7\x10\xfb\xe0j\x86\x02`K2./test/devel/remote_control/remote_file.py')

    value = self.shell.command_request([_cmd_shell_get_next_file_info])
    self.assertEqual(value, b'\x00')
