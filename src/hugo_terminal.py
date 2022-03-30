#!/usr/bin/env python

import sys
import argparse
import time
import asyncio
import logging
import uuid
from bleak import BleakClient, BleakScanner
from colorlog import ColoredFormatter
import hashlib
import os
import keyboard
import subprocess
#import toga

class Commands:
  SHELL_VERSION               = 0x80
  SHELL_STOP_PROGRAM          = 0x81
  SHELL_START_PROGRAM         = 0x82
  SHELL_GET_NEXT_FILE_INFO    = 0x83
  SHELL_REMOVE_FILE           = 0x84
  SHELL_HANDLE_FILE           = 0x85
  SHELL_GET_FILE_CHECKSUM     = 0x86
  SHELL_APPEND                = 0x87
  SHELL_MK_DIR                = 0x88

class Ble():
  ble_message_max_size = 512
  command_uuid = uuid.UUID("48754770-0000-1000-8000-00805F9B34FB")
  log_uuid = uuid.UUID("48754771-0000-1000-8000-00805F9B34FB")
  keyboard_uuid = uuid.UUID("48754772-0000-1000-8000-00805F9B34FB")
  property_uuid = uuid.UUID("48754773-0000-1000-8000-00805F9B34FB")

  def __init__(self, hook_keyboard, mac_address):
    self.hook_keyboard = hook_keyboard
    self.connected = False
    self.notification_data = None
    self.mac_address = mac_address
    self.server = None
    self.client = None
    self.log_msg = ""
    self.log_level = 0
    self.terminating = False

    self.loop = asyncio.get_event_loop()
    logging.getLogger('bleak').setLevel(logging.CRITICAL)

  def _disconnected_callback(self, client):
    logging.debug("disconnected callback for: %s", client)
    self.connected = False

  def _command_notyfy_callback(self, _sender: int, data: bytearray):
    self.notification_data = data

  async def _command(self, command_id, data, wait_for_answer):
    payload = bytes([command_id])
    payload += data

    self.notification_data = None
    _answer = await self.client.write_gatt_char(self.command_uuid, payload, not wait_for_answer)
    if wait_for_answer:
      while not self.notification_data:
        await asyncio.sleep(0.01)

    return self.notification_data

  def command(self, command_id, data, wait_for_answer=True):
    if len(data) > self.ble_message_max_size:
      logging.error("try to send too long data %s for command: %d", data.hex(), command_id)
      return None
    else:
      return self.loop.run_until_complete(self._command(command_id, data, wait_for_answer))

  async def _key_pressed(self, scan_code, key_name):
    data = scan_code.to_bytes(2, byteorder='big', signed=True) + key_name.encode("utf-8")
    await self.client.write_gatt_char(self.keyboard_uuid, data, False)

  def key_pressed(self, scan_code, key_name):
    self.loop.create_task(self._key_pressed(scan_code, key_name))

  def _log_callback(self, _sender: int, data: bytearray):
    is_first = not self.log_msg
    if is_first:
      level = data[0]

    is_last = data[-1] != '\f'.encode("utf-8")[0]
    self.log_msg += data[(1 if is_first else 0) : (len(data) if is_last else -1)].decode("utf-8") # skip level and \f

    if is_last:
      logging.log(level, self.log_msg)
      self.log_msg = ""

  def _detection_callback(self, device, _advertisement_data):
    if device.name and device.name.startswith("HuGo"):
      reduced_address = device.address.replace(':', '')
      if not self.mac_address or self.mac_address.lower() == reduced_address.lower():
        self.server = device.address
      logging.debug("HuGo has been found: '%s, %s'", device.name, device.address)

  async def _scan(self):
    logging.info("Scanning...")
    scanner = BleakScanner()
    scanner.register_detection_callback(self._detection_callback)

    logging.debug("searching HuGo device")
    waiting = 0.5 # to be cough power_up window in case of power save
    countdown = int(90 / waiting) #~1.5 min
    self.server = None
    while countdown:
      await scanner.start()
      await asyncio.sleep(waiting)
      await scanner.stop()

      if self.server:
        logging.debug("server found %s", self.server )
        logging.info("Scanning DONE")
        return True

      countdown -= 1
    logging.info("Scanning FAILED")
    return False


  async def _connect(self):
    if not self.server:
      if not await self._scan():
        logging.error("HuGo not found")
        return False

    logging.info("connecting to %s", str(self.server))
    for _i in range(9): # ~90 sec
      try:
        self.client = BleakClient(self.server, loop=self.loop) #it is better to create client again when the connection fails. in some cases the connections is created partially and is not possible to establish the new one
        self.connected = await self.client.connect()
        logging.debug("device was connected with status: %s", self.connected)
        if not self.connected:
          return False

        self.client.set_disconnected_callback(self._disconnected_callback)
        await self.client.start_notify(self.command_uuid, self._command_notyfy_callback)
        #FIXME: should be removed when logging is finished
        try:
          await self.client.start_notify(self.log_uuid, self._log_callback)
        except Exception as error:
          logging.warning("logging start_notify failed %s", error)
        return True
      except Exception as e:
        logging.debug("connection error: %s", e)
        logging.debug("device was not connected via BLE")
    return False


  def connect(self):
    return self.loop.run_until_complete(self._connect())

  def disconnect(self):
    logging.info("disconnecting...")
    self.loop.run_until_complete(self.client.disconnect())
    logging.info("disconnecting DONE")
    self.connected = False

  def keyboard_monitor(self, key_event):
    if key_event.name == "esc":
      keyboard.unhook_all()
      self.terminating = True
    else:
      self.key_pressed(key_event.scan_code, key_event.name)

  async def _async_monitor(self):
    if self.hook_keyboard:
      logging.info("Keyboard monitoring. Press 'Esc' to finish.")
      keyboard.on_press(self.keyboard_monitor, suppress=True)

    while True:
      if self.terminating:
        logging.debug("terminating...")
        break

      if not self.connected:
        await self._connect()

      await asyncio.sleep(0.1)

  def monitor(self):
    try:
      self.loop.run_until_complete(self._async_monitor())
    except KeyboardInterrupt:
      self.terminating = True

class Terminal():
  output_dir = ".build"
  def __init__(self, hook_keyboard, mac_address, verbose, source_dir):
    self.flashing_folder = source_dir
    self._init_logger(logging.DEBUG if verbose else logging.INFO)
    self.ble = Ble(hook_keyboard, mac_address)

  def _init_logger(self, log_level):
    REMOTE_CRITICAL = 51
    REMOTE_ERROR    = 41
    REMOTE_WARNING  = 31
    REMOTE_INFO     = 21
    REMOTE_DEBUG    = 11

    logging.addLevelName(REMOTE_CRITICAL, "=CRITICAL")
    logging.addLevelName(REMOTE_ERROR, "=ERROR")
    logging.addLevelName(REMOTE_WARNING, "=WARNING")
    logging.addLevelName(REMOTE_INFO, "=INFO")
    logging.addLevelName(REMOTE_DEBUG, "=DEBUG")

    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(ColoredFormatter(
        "%(log_color)s%(levelname)-9s%(reset)s%(message)s",
        log_colors={
          "CRITICAL": "red,bg_white",
          "ERROR": "red",
          "WARNING": "yellow",
          "INFO": "green",
          "DEBUG": "white",
          "=CRITICAL": "bold_red,bg_white",
          "=ERROR": "bold_red",
          "=WARNING": "bold_yellow",
          "=INFO": "bold_green",
          "=DEBUG": "bold_white"
        }
    ))
    stream_handler.setLevel(log_level)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    logger.setLevel(log_level)

  def connect(self):
    if not self.ble.connect():
      logging.error("not connected")
      return False
    return True

  def disconnect(self):
    self.ble.disconnect()
    while self.ble.connected:
      time.sleep(0.1)

  def stop_program(self):
    logging.info("terminating_program...")
    self.ble.command(Commands.SHELL_STOP_PROGRAM, b"") #terminate
    try:
      self.ble.disconnect()
    except Exception:
      pass
    if not self.connect():
      return False
    logging.info("terminating_program DONE")
    return True

  def start_program(self):
    logging.info("starting program...")
    ret_val = self.ble.command(Commands.SHELL_START_PROGRAM, b"") #start program
    if ret_val == b"\1":
      logging.info("starting program DONE")
    else:
      logging.warning("program terminated!")

  def _get_remote_files(self):
    remote_files = {}
    logging.debug("getting remote files...")
    while True:
      self.ble.notification_data = None
      file_info = self.ble.command(Commands.SHELL_GET_NEXT_FILE_INFO, b"")

      if file_info == b"\0":
        break
      remote_files[file_info[20:].decode("utf-8")] = file_info[:20].hex()
    logging.debug("getting remote files Done")
    return remote_files

  def _get_local_files(self, directory):
    local_files = {}
    dir_content = os.listdir(directory)
    for file_name in dir_content:
      file_path = directory + "/" + file_name
      if os.path.isdir(file_path):
        local_files.update(self._get_local_files(file_path))
      else:
        with open(file_path, "rb") as file:
          data = file.read()
          local_files[file_path[len(self.output_dir):]] = hashlib.sha1(data).digest().hex()
    return local_files

  def _remove_unnecessary_files(self, remote_files, local_files):
    logging.debug("removing unnecessary files ...")
    removed = False
    for file_path in remote_files:
      self.ble.notification_data = None
      if file_path not in local_files:
        logging.info("removing file %s", file_path)
        self.ble.command(Commands.SHELL_REMOVE_FILE, file_path.encode("utf-8"))
        removed = True
    logging.debug("removing unnecessary files Done")
    return removed

  def _import_files(self, remote_files, local_files):
    logging.debug("importing files ...")
    imported = False
    for file_path in local_files:
      self.ble.notification_data = None
      if file_path not in remote_files or remote_files[file_path] != local_files[file_path]:
        imported = True
        logging.info("uploading file %s", file_path)

        path_elements = file_path[1:].split("/") #file  path start by /
        if len(path_elements) > 1:
          path_elements = path_elements[:-1]
          path = ""
          for element in path_elements:
            path += "/" + element
            self.ble.command(Commands.SHELL_MK_DIR, path.encode("utf-8"))

        with open(self.output_dir + "/" + file_path, "rb") as f:
          data = f.read()
          self.ble.notification_data = None
          self.ble.command(Commands.SHELL_HANDLE_FILE, file_path.encode("utf-8"))
          step = self.ble.ble_message_max_size - 1 # - command header
          for i in range(0, len(data), step):
            self.ble.command(Commands.SHELL_APPEND, data[i: i + step])
        stored_file_checksum = self.ble.command(Commands.SHELL_GET_FILE_CHECKSUM, b"")
        if stored_file_checksum.hex() != local_files[file_path]:
          logging.error("file %s has not been stored properly: %s", file_path, str((stored_file_checksum.hex(), local_files[file_path])))
    logging.debug("importing files Done")
    return imported

  def _remove_old_dir_content(self, in_dir_content, out_dir_path):
    dir_content = os.listdir(out_dir_path)
    for out_file_name in dir_content:
      unbuild_name = out_file_name.replace(".mpy", ".py")
      if unbuild_name not in in_dir_content:
        out_file_path = out_dir_path + "/" + out_file_name
        if os.path.isdir(out_file_path):
          self._remove_old_dir_content([], out_file_path)
          os.rmdir(out_file_path)
        else:
          os.remove(out_file_path)

  def _update_required(self, in_path, out_path):
    if not os.path.exists(out_path):
      return True
    return os.path.getmtime(in_path) > os.path.getmtime(out_path)

  def _build_local_dir(self, in_path, out_path):
    in_dir_content = os.listdir(in_path)

    if not os.path.exists(out_path):
      os.mkdir(out_path)
    else:
      self._remove_old_dir_content(in_dir_content, out_path)

    for file_name in in_dir_content:
      if file_name in ("__init__.py"):
        continue

      in_file_path = in_path + "/" + file_name
      if os.path.isdir(in_file_path):
        if file_name != "__pycache__":
          self._build_local_dir(in_file_path, out_path + "/" + file_name)
        continue

      if file_name == "boot.py" or not file_name.endswith(".py"):
        if self._update_required(in_file_path, self.output_dir + "/" + file_name):
          os.system(f"cp {in_file_path} { self.output_dir}")
      else:
        out_file_path = out_path + "/" + file_name.split(".")[0] + ".mpy"
        if self._update_required(in_file_path, out_file_path):
          try:
            subprocess.run(f"mpy-cross {in_file_path} -o {out_file_path}", check=True, shell=True, stderr=subprocess.PIPE)
          except subprocess.CalledProcessError as error:
            logging.warning("build of '%s' unsuccessful: %s", str(in_file_path), error.stderr.decode("utf-8"))

  def _process_files(self):
    remote_files = self._get_remote_files()

    logging.debug("building local files...")
    self._build_local_dir(self.flashing_folder, self.output_dir)
    logging.debug("building local files Done")

    local_files = self._get_local_files(self.output_dir)
    changed = self._remove_unnecessary_files(remote_files, local_files)
    changed |= self._import_files(remote_files, local_files)
    return changed

  def flash_files(self):
    logging.info("Uploading...")
    timestamp = time.time()
    flashed = self._process_files()
    if flashed:
      logging.info("uploading time: %d s", time.time() - timestamp)

    if flashed:
      logging.info("Uploading DONE")
    else:
      logging.info("All files are up-to-date")
    return True

  def monitor(self):
    logging.info("Monitoring...")
    self.ble.monitor()
    logging.info("Monitoring Terminated")

def process_cmd_arguments():
  parser = argparse.ArgumentParser(description='This tool provide a BLE communication interface for HuGo module kit')

  parser.add_argument('--flash', '-f', action='store_true', help=f'Uploads content of folder specified by -s argument. When the -s argument is not entered "upload" folder will be used. Deprecated content will be removed.')
  parser.add_argument('--mac_addr', '-a', help='mac address of the main HuGo module')
  parser.add_argument('--monitor', '-m', action='store_true', help='Stay connected to receive log messages.')
  parser.add_argument('--remote_control', '-rc', action='store_true', help='read pressed keys and provide them as a remore control.')
  parser.add_argument('--verbose', '-v', action='store_true', help='debug messages are printed')
  parser.add_argument('--source_dir', '-s', default="upload", help='defines path to directory containing events.py which will be uploaded. If the source dir is not defined, the"upload" folder will be used.')
  parser.add_argument('--extra_reboot', '-xr', action='store_true', help='perform an extra reboot after flashing. It is usefull when a core scripts that are already loaded are changed')


  args = parser.parse_args()
  if not args.flash and not args.monitor:
    parser.print_help()
    return None

  return args

# def button_handler(widget):
#     print("hello")


# def build(app):
#   box = toga.Box()

  # button = toga.Button('Hello world', on_press=button_handler)
  # button.style.padding = 50
  # button.style.flex = 1
  # box.add(button)

  return box

def main():
  args = process_cmd_arguments()
  if not args:
    return

  if not args.flash and not args.monitor:
    return True

  # app = toga.App(
  #       'HuGo terminal',
  #       'org.hugo.terminal',
  #       author='Jakub Vesely',
  #       description="service app for HuGo project",
  #       version="1",
  #       home_page="https://github.com/jakub-vesely/HuGo",
  #       startup=build
  #   )
  #app.main_loop()

  terminal = Terminal(args.remote_control, args.mac_addr, args.verbose, args.source_dir)
  if not terminal.connect():
    return False

  if not terminal.stop_program():
    return False

  if args.flash:
    if not terminal.flash_files():
      return False

    if args.extra_reboot:
      if not terminal.stop_program():
        return False

  terminal.start_program()

  if args.monitor:
    terminal.monitor()

  terminal.disconnect()
  return True

if not main():
  sys.exit(1)
