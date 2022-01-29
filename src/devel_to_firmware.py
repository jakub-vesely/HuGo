#!/usr/bin/env python

import os

def main():
  devel_prefix = "___"
  devel_folder_path = "devel/"
  firmware_folder_path = "../micropython/ports/esp32/boards/HUGO/modules/"
  devel_only_file_names = ("boot.py", "events.py")
  file_names = os.listdir(devel_folder_path)
  for file_name in file_names:
    if file_name in (devel_only_file_names):
      continue
    if os.path.isdir(devel_folder_path + "/" + file_name):
      continue

    firmware_file_name = file_name.replace(devel_prefix, "")
    with open(devel_folder_path + file_name, "r") as devel_file:
      devel_lines = devel_file.readlines()
      firmware_lines = list()
      for line in devel_lines:
        firmware_lines.append(line.replace(devel_prefix, ""))
      with open(firmware_folder_path + firmware_file_name, "w") as firmware_file:
        firmware_file.writelines(firmware_lines)

main()