import sys
import datetime
import struct
import time
import asyncio
from asyncio import futures
import json
import logging
import uuid
from bleak import BleakClient, BleakError, BleakScanner
import txdbus
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

  def __init__(self):
    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(ColoredFormatter("%(log_color)s%(levelname)-8s%(reset)s%(message)s"))
    stream_handler.setLevel(logging.INFO)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    logger.setLevel(logging.INFO)
    self.connected = False
    self.notification_data = None

    self.loop = asyncio.new_event_loop()
    logging.getLogger('bleak').setLevel(logging.CRITICAL)

  def _disconnected_callback(self, client):
    logging.debug("disconnected callback for: %s", client)
    self.connected = False

  async def _notyfy_callback(self, sender: int, data: bytearray):
    logging.debug("notification from: %d, %s", sender, str(data))
    self.notification_data = data

  async def _get_value_notyfy_callback(self, sender: int, data: bytearray):
    self.notification_data = data

  async def _get_value(self, property_id):
    self.notification_data = None
    answer = await self.client.write_gatt_char(self.get_property_uuid, bytes([property_id]), False)
    while not self.notification_data:
      await asyncio.sleep(0.01)

  def get_value(self, property_id):
    return self.loop.run_until_complete(self._get_value(property_id))

  async def _set_value_notyfy_callback(self, sender: int, data: bytearray):
    self.notification_data = data

  async def _set_value(self, property_id, value):
    data = bytes([property_id])
    data += struct.pack("d", value)

    self.notification_data = None
    answer = await self.client.write_gatt_char(self.set_property_uuid, data, False)
    while not self.notification_data:
      await asyncio.sleep(0.01)

  def set_value(self, property_id, value):
    return self.loop.run_until_complete(self._set_value(property_id, value))


  def _command_notyfy_callback(self, sender: int, data: bytearray):
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

  async def _connect(self, address):
    self.client = BleakClient(address, loop=self.loop)
    try:
      connected = await self.client.connect()
      logging.debug("device was connected with status: %s", connected)
      if not connected:
        return False

      self.client.set_disconnected_callback(self._disconnected_callback)
      await self.client.start_notify(self.command_uuid, self._command_notyfy_callback)

    except Exception as e:
      logging.debug("connection error: %s", e)
      logging.debug("device was not connected via BLE")
      return False
    return True

  def connect(self):
    logging.info("connecting to %s", str(self.server))
    self.connected = self.loop.run_until_complete(self._connect(self.server))
    return self.connected

  def _detection_callback(self, device, _advertisement_data):
    if device.name and device.name.startswith("HuGo"):
      self.server = device.address
      logging.debug("HuGo has been found: '%s'", device.name)

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
        return self.server

      countdown -= 1

  def scan(self):
    return self.loop.run_until_complete(self._scan())

  def disconnect(self):
    self.loop.run_until_complete(self.client.disconnect())
    self.connected = False

def stop_program():
  logging.info("terminating_program...")
  ble.command(Commands.SHELL_STOP_PROGRAM, b"") #terminate
  try:
    ble.disconnect()
  except Exception:
    pass
  ble.connect()
  logging.debug("terminating_program DONE")

def start_program():
  logging.info("starting program...")
  error = ble.command(Commands.SHELL_START_PROGRAM, b"") #start program
  if error == b"\1":
    logging.info("starting program DONE")
  else:
    logging.warning("program crashed with error: \n%s", error.decode("utf-8"))
def get_remote_files():
  remote_files = {}
  while True:
    ble.notification_data = None
    file_info = ble.command(Commands.SHELL_GET_NEXT_FILE_INFO, b"")

    if file_info == b"\0":
      break
    remote_files[file_info[20:].decode("utf-8")] = file_info[:20].hex()
  return remote_files

def get_local_files():
  local_files = {}
  dir_content = os.listdir("./export")
  for file_name in dir_content:
    with open("./export/" + file_name, "rb") as f:
      data = f.read()
      local_files[file_name] = hashlib.sha1(data).digest().hex();
  return local_files

def remove_unncesarry_files(remote_files, local_files):
  removed = False
  for file_name in remote_files:
    ble.notification_data = None
    if file_name not in local_files:
      logging.info("removing file %s", file_name)
      ble.command(Commands.SHELL_REMOVE_FILE, file_name.encode("utf-8"))
      removed = True
  return removed

def import_files(remote_files, local_files):
  imported = False
  for file_name in local_files:
    ble.notification_data = None
    if file_name not in remote_files or remote_files[file_name] != local_files[file_name]:
      imported = True
      logging.info(f"uploading file {file_name}")
      with open("./export/" + file_name, "rb") as f:
        data = f.read()
        ble.notification_data = None
        ble.command(Commands.SHELL_HANDLE_FILE, file_name.encode("utf-8"))
        step = ble.ble_message_max_size - 1 # - command header
        for i in range(0, len(data), step):
          ble.command(Commands.SHELL_APPEND, data[i: i + step])
      stored_file_checksum = ble.command(Commands.SHELL_GET_FILE_CHECKSUM, b"")
      if stored_file_checksum.hex() != local_files[file_name]:
        logging.error("file %s has not been stored properly: %s", file_name, str((stored_file_checksum.hex(), local_files[file_name])))
  return imported


def process_files():
  remote_files = get_remote_files()
  local_files = get_local_files()
  changed = remove_unncesarry_files(remote_files, local_files)
  changed |= import_files(remote_files, local_files)
  return changed

ble = Ble()
if not ble.scan():
  logging.error("HuGo not found")
else:
  if not ble.connect():
    logging.error("not connected")
  else:
    stop_program()
    timestamp = time.time()
    uploaded = process_files()
    if uploaded:
      logging.info("uploading time: %d s", time.time() - timestamp)

    start_program()
    if uploaded:
      logging.info("Uploading DONE")
    else:
      logging.info("All files are up-to-date")

  ble.disconnect()
while ble.connected:
  time.sleep(0.1)