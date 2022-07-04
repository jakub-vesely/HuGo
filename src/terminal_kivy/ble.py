import asyncio
import logging
import uuid
from bleak import BleakClient, BleakScanner
import keyboard

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
      self.log_level = data[0]

    is_last = data[-1] != '\f'.encode("utf-8")[0]
    self.log_msg += data[(1 if is_first else 0) : (len(data) if is_last else -1)].decode("utf-8") # skip level and \f

    if is_last:
      logging.log(self.log_level, self.log_msg)
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
