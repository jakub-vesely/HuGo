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

class Commands:
  SHELL_VERSION               = 0x80
  SHELL_STOP_PROGRAM          = 0x81
  SHELL_START_PROGRAM         = 0x82
  SHELL_GET_NEXT_FILE_INFO    = 0x83
  SHELL_REMOVE_FILE           = 0x84
  SHELL_HANDLE_FILE           = 0x85
  SHELL_GET_FILE_CHECKSUM     = 0x86
  SHELL_APPEND                = 0x87

class Ble():
  ble_message_max_size = 512
  get_property_uuid = uuid.UUID("48754770-0000-1000-8000-00805F9B34FB")
  set_property_uuid = uuid.UUID("48754771-0000-1000-8000-00805F9B34FB")
  command_uuid = uuid.UUID("48754772-0000-1000-8000-00805F9B34FB")
  log_uuid = uuid.UUID("48754773-0000-1000-8000-00805F9B34FB")

  def __init__(self, mac_address):
    self.connected = False
    self.notification_data = None
    self.mac_address = mac_address
    self.server = None
    self.client = None
    self.log_msg = ""
    self.log_level = 0
    self.terminating = False

    self.loop = asyncio.new_event_loop()
    logging.getLogger('bleak').setLevel(logging.CRITICAL)

  def _disconnected_callback(self, client):
    logging.debug("disconnected callback for: %s", client)
    self.connected = False

  # async def _notyfy_callback(self, sender: int, data: bytearray):
  #   logging.debug("notification from: %d, %s", sender, str(data))
  #   self.notification_data = data

  # async def _get_value_notyfy_callback(self, sender: int, data: bytearray):
  #   self.notification_data = data

  # async def _get_value(self, property_id):
  #   self.notification_data = None
  #   answer = await self.client.write_gatt_char(self.get_property_uuid, bytes([property_id]), False)
  #   while not self.notification_data:
  #     await asyncio.sleep(0.01)

  # def get_value(self, property_id):
  #   return self.loop.run_until_complete(self._get_value(property_id))

  # async def _set_value_notyfy_callback(self, sender: int, data: bytearray):
  #   self.notification_data = data

  # async def _set_value(self, property_id, value):
  #   data = bytes([property_id])
  #   data += struct.pack("d", value)

  #   self.notification_data = None
  #   answer = await self.client.write_gatt_char(self.set_property_uuid, data, False)
  #   while not self.notification_data:
  #     await asyncio.sleep(0.01)

  # def set_value(self, property_id, value):
  #   return self.loop.run_until_complete(self._set_value(property_id, value))


  def _command_notyfy_callback(self, _sender: int, data: bytearray):
    self.notification_data = data

  async def _command(self, command_id, data, wait_for_answer):
    payload = bytes([command_id])
    payload += data

    self.notification_data = None
    answer = await self.client.write_gatt_char(self.command_uuid, payload, not wait_for_answer)
    if wait_for_answer:
      while not self.notification_data:
        await asyncio.sleep(0.01)

    return self.notification_data

  def command(self, command_id, data, wait_for_answer=True):
    if len(data) > self.ble_message_max_size:
      logging.error("try to send too long data %s for command: %", data.hex(), command_id)
      return None
    else:
      return self.loop.run_until_complete(self._command(command_id, data, wait_for_answer))

  def _log_callback(self, _sender: int, data: bytearray):
    is_first = not self.log_msg
    if is_first:
      self.level = data[0]

    is_last = data[-1] != '\f'.encode("utf-8")[0]
    self.log_msg += data[(1 if is_first else 0) : (len(data) if is_last else -1)].decode("utf-8") # skip level and \f

    if is_last:
      logging.log(self.level, self.log_msg)
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
    countdown = 30
    self.server = None
    while countdown:
      await scanner.start()
      await asyncio.sleep(1)
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
    self.client = BleakClient(self.server, loop=self.loop)
    for _i in range(3):
      try:
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
        logging.warning("connection error: %s", e)
        logging.debug("device was not connected via BLE")
    return False


  def connect(self):
    return self.loop.run_until_complete(self._connect())

  def disconnect(self):
    logging.info("disconnecting...")
    self.loop.run_until_complete(self.client.disconnect())
    logging.info("disconnecting DONE")
    self.connected = False

  async def _async_monitor(self):
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
  flashing_folder = "./export"
  def __init__(self, mac_address, verbose):
    self._init_logger(logging.DEBUG if verbose else logging.INFO)
    self.ble = Ble(mac_address)

  def _init_logger(self, log_level):
    REMOTE_CRITICAL = 51
    REMOTE_ERROR    = 41
    REMOTE_WARNING  = 31
    REMOTE_INFO     = 21
    REMOTE_DEBUG    = 11

    logging.addLevelName(REMOTE_CRITICAL, " CRITICAL")
    logging.addLevelName(REMOTE_ERROR, " ERROR")
    logging.addLevelName(REMOTE_WARNING, " WARNING")
    logging.addLevelName(REMOTE_INFO, " INFO")
    logging.addLevelName(REMOTE_DEBUG, " DEBUG")

    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(ColoredFormatter(
        "%(log_color)s%(levelname)-9s%(reset)s%(message)s",
        log_colors={
          "CRITICAL": "red,bg_white",
          "ERROR": "red",
          "WARNING": "yellow",
          "INFO": "green",
          "DEBUG": "white",
          " CRITICAL": "bold_red,bg_white",
          " ERROR": "bold_red",
          " WARNING": "bold_yellow",
          " INFO": "bold_green",
          " DEBUG": "bold_white"
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
    while True:
      self.ble.notification_data = None
      file_info = self.ble.command(Commands.SHELL_GET_NEXT_FILE_INFO, b"")

      if file_info == b"\0":
        break
      remote_files[file_info[20:].decode("utf-8")] = file_info[:20].hex()
    return remote_files

  def _get_local_files(self):
    local_files = {}
    dir_content = os.listdir(self.flashing_folder)
    for file_name in dir_content:
      with open(self.flashing_folder + "/" + file_name, "rb") as f:
        data = f.read()
        local_files[file_name] = hashlib.sha1(data).digest().hex();
    return local_files

  def _remove_unncesarry_files(self, remote_files, local_files):
    removed = False
    for file_name in remote_files:
      self.ble.notification_data = None
      if file_name not in local_files:
        logging.info("removing file %s", file_name)
        self.ble.command(Commands.SHELL_REMOVE_FILE, file_name.encode("utf-8"))
        removed = True
    return removed

  def _import_files(self, remote_files, local_files):
    imported = False
    for file_name in local_files:
      self.ble.notification_data = None
      if file_name not in remote_files or remote_files[file_name] != local_files[file_name]:
        imported = True
        logging.info(f"flashing file {file_name}")
        with open(self.flashing_folder + "/" + file_name, "rb") as f:
          data = f.read()
          self.ble.notification_data = None
          self.ble.command(Commands.SHELL_HANDLE_FILE, file_name.encode("utf-8"))
          step = self.ble.ble_message_max_size - 1 # - command header
          for i in range(0, len(data), step):
            self.ble.command(Commands.SHELL_APPEND, data[i: i + step])
        stored_file_checksum = self.ble.command(Commands.SHELL_GET_FILE_CHECKSUM, b"")
        if stored_file_checksum.hex() != local_files[file_name]:
          logging.error("file %s has not been stored properly: %s", file_name, str((stored_file_checksum.hex(), local_files[file_name])))
    return imported


  def _process_files(self):
    remote_files = self._get_remote_files()
    local_files = self._get_local_files()
    changed = self._remove_unncesarry_files(remote_files, local_files)
    changed |= self._import_files(remote_files, local_files)
    return changed

  def flash_files(self):
    timestamp = time.time()
    flashed = self._process_files()
    if flashed:
      logging.info("flashing time: %d s", time.time() - timestamp)

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
  flash = False
  monitor = False
  mac_address = None

  parser = argparse.ArgumentParser(description='This tool provide a BLE communication interface for HuGo module kit')

  parser.add_argument('--flash', '-f', action='store_true', help=f'Uploads content of the"{Terminal.flashing_folder}" directory to device. Deprecated file will be removed.')
  parser.add_argument('--monitor', '-m', action='store_true', help='Stay conected to receive log messages.')
  parser.add_argument('--mac_address', '-a', help='mac address of the main HuGo module')
  parser.add_argument('--verbose', '-v', action='store_true', help='debug messages are printed')
  args = parser.parse_args()
  if not args.flash and not args.monitor:
    parser.print_help()
    return(None, None, None, None)

  return (args.flash, args.monitor, args.mac_address, args.verbose)

def main():
  flash, monitor, mac_address, verbose = process_cmd_arguments()
  if not flash and not monitor:
    return True

  terminal = Terminal(mac_address, verbose)
  if not terminal.connect():
    return False

  if not terminal.stop_program():
    return False

  if flash:
    if not terminal.flash_files():
      return False

  terminal.start_program()

  if monitor:
    terminal.monitor()

  terminal.disconnect()

if not main():
  sys.exit(1)