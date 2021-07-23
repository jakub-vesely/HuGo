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
  get_property_uuid = uuid.UUID("48754770-0000-1000-8000-00805F9B34FB")
  set_property_uuid = uuid.UUID("48754771-0000-1000-8000-00805F9B34FB")
  command_uuid = uuid.UUID("48754772-0000-1000-8000-00805F9B34FB")

  def __init__(self):
    stream_handler = logging.StreamHandler()
    stream_handler.setFormatter(ColoredFormatter("%(log_color)s%(levelname)-8s%(reset)s%(message)s"))
    stream_handler.setLevel(logging.WARN)
    logger = logging.getLogger("")
    logger.addHandler(stream_handler)
    logger.setLevel(logging.WARN)
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

  # async def _read(self):
  #   logging.debug("reading...")
  #   answer = await self.client.read_gatt_char(self.property_uuid)
  #   logging.debug("reading done: %s", answer)

  # def read(self):
  #   return self.loop.run_until_complete(self._read())

  # async def _write(self):
  #   logging.debug("writintg...")
  #   answer = await self.client.write_gatt_char(self.property_uuid, b"\02\00\00\00", False)
  #   logging.debug("writing done: %s", answer)
  #   await self.client.disconnect()
  #   self.connected = False

  # def write(self):
  #   return self.loop.run_until_complete(self._write())

  async def _get_value_notyfy_callback(self, sender: int, data: bytearray):
    print(("_get_value_notyfy_callback", sender, data))
    logging.debug("received_data from: %d, %s", sender, str(struct.unpack("d", data)))
    self.notification_data = data

  async def _get_value(self, property_id):

    logging.debug("getting value for " + str(property_id))

    self.notification_data = None
    answer = await self.client.write_gatt_char(self.get_property_uuid, bytes([property_id]), False)
    while not self.notification_data:
      await asyncio.sleep(0.01)
    logging.debug("writing done: %s", answer)


  def get_value(self, property_id):
    return self.loop.run_until_complete(self._get_value(property_id))

  async def _set_value_notyfy_callback(self, sender: int, data: bytearray):
    print(("_set_value_notyfy_callback", sender, data))
    logging.debug("received_data from: %d, %s", sender, str(data))
    self.notification_data = data

  async def _set_value(self, property_id, value):

    logging.debug("setting value for " + str(property_id))

    data = bytes([property_id])
    data += struct.pack("d", value)
    #print("prepared_data %s", data)

    self.notification_data = None
    answer = await self.client.write_gatt_char(self.set_property_uuid, data, False)
    while not self.notification_data:
      await asyncio.sleep(0.01)

    logging.debug("writing done: %s", answer)


  def set_value(self, property_id, value):
    return self.loop.run_until_complete(self._set_value(property_id, value))


  async def _command_notyfy_callback(self, sender: int, data: bytearray):
    #print(("_command_notyfy_callback", sender, data))
    logging.debug("received_data from: %d, %s", sender, str(data))
    self.notification_data = data

  async def _command(self, command_id, data):

    logging.debug("command for " + str(command_id))

    payload = bytes([command_id])
    payload += data
    #print("prepared_data", payload)

    self.notifnotification_dataied = None
    answer = await self.client.write_gatt_char(self.command_uuid, payload, False)
    while not self.notification_data:
      await asyncio.sleep(0.01)

    logging.debug("writing done: %s", answer)

  def command(self, command_id, data):
    return self.loop.run_until_complete(self._command(command_id, data))

  async def _connect(self, address):
    logging.debug("connecting...")
    self.client = BleakClient(address, loop=self.loop)
    try:
      connected = await self.client.connect()
      logging.debug("device was connected with status: %s", connected)
      if not connected:
        return False

      self.client.set_disconnected_callback(self._disconnected_callback)
      await self.client.start_notify(self.get_property_uuid, self._get_value_notyfy_callback)
      await self.client.start_notify(self.set_property_uuid, self._set_value_notyfy_callback)
      await self.client.start_notify(self.command_uuid, self._command_notyfy_callback)

    except Exception as e:
      logging.debug("connection error: %s", e)
      logging.debug("device was not connected via BLE")
      return False
    return True

  def connect(self):
    self.connected = self.loop.run_until_complete(self._connect(self.server))
    return self.connected

  def _detection_callback(self, device, _advertisement_data):
    print(device)
    if device.name and device.name.startswith("HuGo"):
      self.server = device.address
      logging.debug("HuGo has been found: '%s'", device.name)

  async def _scan(self):
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

with open("tiny_block_rgb_led.lua", "rb") as f:
  #print(hashlib.md5(b"hello-world").digest().hex())
  data = f.read()
  print(hashlib.md5(data).digest().hex())

#sys.exit(0)
def stop_program():
  print("terminating_program...")
  ble.command(Commands.SHELL_STOP_PROGRAM, b"") #terminate
  ble.disconnect()
  ble.connect()
  print("terminating_program DONE")

def start_program():
  print("starting_program...")
  ble.command(Commands.SHELL_START_PROGRAM, b"") #start program
  ble.disconnect()
  ble.connect()
  print("starting_program DONE")

def get_remote_files():
  remote_files = {}
  while True:
    ble.notification_data = None
    ble.command(Commands.SHELL_GET_NEXT_FILE_INFO, b"")
    if ble.notification_data == b"\0":
      break
    remote_files[ble.notification_data[16:].decode("utf-8")] = ble.notification_data[:16].hex();
  return remote_files

def get_local_files():
  local_files = {}
  dir_content = os.listdir("./lua")
  for file_name in dir_content:
    with open("./lua/" + file_name, "rb") as f:
      data = f.read()
      local_files[file_name] = hashlib.md5(data).digest().hex();
  return local_files

def remove_unncesarry_files(remote_files, local_files):
  for file_name in remote_files:
    ble.notification_data = None
    if file_name not in local_files:
      print("removing file ", file_name)
      ble.command(0x84, file_name.encode("utf-8")) #remove file

      while ble.notification_data == None:
        time.sleep(0.1)

def import_files(remote_files, local_files):
  for file_name in local_files:
    ble.notification_data = None
    if file_name not in remote_files or remote_files[file_name] != local_files[file_name]:
      print(f"adding file {file_name}")
      with open("./lua/" + file_name, "rb") as f:
        data = f.read()
        ble.notification_data = None
        ble.command(0x85, file_name.encode("utf-8")) #handle
        while ble.notification_data == None:
          time.sleep(0.1)
        step = 400
        for i in range(0, len(data), step):
          ble.notification_data = None
          ble.command(0x87, data[i: i + step]) #add file
          while ble.notification_data == None:
            time.sleep(0.1)

def process_files():
  remote_files = get_remote_files()
  local_files = get_local_files()
  remove_unncesarry_files(remote_files, local_files)
  import_files(remote_files, local_files)

ble = Ble()
ble.scan()
if not ble.connect():
  print("not connected")
else:
  #while not ble.notified:
  #  time.sleep(1)

  stop_program()
  process_files()
  start_program()

  #ble.command(0x84, b"/lua/tiny_block_rgb_led.lua")
  #ble.command(0x85, b"")
  #ble.read()
  # while True:
  #   now = datetime.datetime.now()
  #   hours = now.hour
  #   minuts = now.minute
  #   seconds = now.second
  #   print (f"{hours}:{minuts}:{seconds}")

  #   ble.set_value(1, hours)
  #   ble.set_value(2, minuts)
  #   ble.set_value(3, seconds)

  #   time.sleep(0.5)

  print("set done")
  print("get")
  #ble.get_value(1)
  ble.disconnect()
while ble.connected:
  time.sleep(1)
