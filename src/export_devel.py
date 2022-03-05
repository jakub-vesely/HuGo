#!/usr/bin/env python

import os

class ExportDevel:
  devel_prefix = "___"
  devel_folder_path = "devel"
  firmware_folder_path = "../micropython/ports/esp32/boards/HUGO/modules"
  devel_only_file_names = ("boot.py", "events.py", "__pycache__")

  def _export_dir(self, path):
    file_names = os.listdir(path)
    for file_name in file_names:
      file_path = path + "/" + file_name
      if file_name in self.devel_only_file_names:
        continue

      if os.path.isdir(file_path):
        self._export_dir(file_path)
        continue

      firmware_folder = self.firmware_folder_path + path[len(self.devel_folder_path):]
      if not os.path.exists(firmware_folder):
        os.mkdir(firmware_folder)

      firmware_file_path = firmware_folder + "/"  + file_name.replace(self.devel_prefix, "")
      with open(file_path, "r") as devel_file:
        devel_lines = devel_file.readlines()
        firmware_lines = list()
        for line in devel_lines:
          firmware_lines.append(line.replace(self.devel_prefix, ""))
        with open(firmware_file_path, "w") as firmware_file:
          firmware_file.writelines(firmware_lines)

  def __init__(self):
    self._export_dir(self.devel_folder_path)

ExportDevel()