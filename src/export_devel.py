#!/usr/bin/env python

#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

import os
import shutil

class ExportDevel:
  devel_prefix = "___"
  devel_folder_path = "devel"
  firmware_folder_path = "../micropython/ports/esp32/boards/HUGO/modules"
  devel_only_file_names = ("boot.py", "events.py", "__pycache__", "__init__.py")

  def _remove_original_files(self):
    shutil.rmtree(self.firmware_folder_path)
    os.mkdir(self.firmware_folder_path)

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
      firmware_folder = firmware_folder.replace(self.devel_prefix, "")
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

  def _create_main_py(self):
    with open(self.firmware_folder_path + "\main.py", "w") as main_file:
      main_file.write("print ('main.py loaded') \nfrom blocks.main_block import MainBlock\n\nMainBlock.run()")

  def __init__(self):
    self._remove_original_files() # remove all original files...
    self._export_dir(self.devel_folder_path) #... to be replaced by new ones
    self._create_main_py()

ExportDevel()