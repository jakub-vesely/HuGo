#!/usr/bin/env python3


import asyncio
from bleak import BleakClient
from dataclasses import dataclass
from queue import Queue
from hugo_dash_board import HugoDashBoard
from hugo_logger import HugoLogger


config_path = "../.hugo-board.json"


# --- CONFIGURATION ---
# Replace with your device's address and the characteristic UUID
DEVICE_ADDRESS = "11:89:9A:A1:A9:34"
NOTIFY_CHARACTERISTIC_UUID = "0000FFE3-0000-1000-8000-00805f9b34fb"
# ---------------------


@dataclass
class FlagByte:
    valid:bool
    response_required:bool
    mode:int
    message_id:int

    def __str__(self):
        return f"{self.valid}, {self.response_required}, {self.mode}, {self.message_id}"


class Meteo:
    def __init__(self, log_queue, device_name, raw_data):
        self.log_queue = log_queue
        self.device_name = device_name
        self.raw_data = raw_data

    @property
    def power_voltage(self):
        #step 0.01 V offset 3 V (3 - 5.55V)
        return self.raw_data[0] / 100 + 3

    @property
    def power_current(self):
        #12 bit signed in mA
        temp = ((self.raw_data[1] & 0b00001111) << 8) + self.raw_data[2]

        if temp & 0x800:
            temp -= 0x1000 # Subtract 2^12 (4096)

        return temp * 0.125

    @property
    def power_charging(self):
        return (self.raw_data[1] >> 7) != 0

    @property
    def ambient_temperature(self):
        #-50 to +50 12 bits - step 0.025
        #rest is unused - can be used later

        temp = ((self.raw_data[3] & 0b00001111) << 8) + self.raw_data[4]

        if temp & 0x800:
            temp -= 0x1000 # Subtract 2^12 (4096)

        return temp * 0.025

    @property
    def ambient_pressure(self):
        # 900-1027; 100 values uint8, offset 900 hpa, step 0.5
        return 900 + self.raw_data[5] * 0.5

    @property
    def ambient_humidity(self):
        #0 - 100%; step 0.4
        return self.raw_data[6] * 0.4

    @property
    def wind_direction(self):

        return int(self.raw_data[7] * 1.4)

    @property
    def wind_speed(self):
        #0 - 127 km/h
        return self.raw_data[8] / 2

    @property
    def  rain_intensity(self):
        #in scopes. result in mm (1 scope 0.2794)
        return   self.raw_data[9] * 0.28

    def _add_log(self, component, quantity, value, unit):
        self.log_queue.put(f"=VALUE {self.device_name}.{component}.{quantity}: {value}" + (f" {unit}" if unit else ""))

    def publish(self, dashboard:HugoDashBoard, has_outdoor:bool, has_co2:bool):
        self._add_log("power", "voltage", self.power_voltage, "V")
        self._add_log("power", "current", self.power_current, "mA")
        self._add_log("power", "charging", self.power_charging, "")
        self._add_log("ambient", "temperature", self.ambient_temperature, "°C")
        self._add_log("ambient", "pressure", self.ambient_pressure, "hPa")
        self._add_log("ambient", "humidity", self.ambient_humidity, "%")

        if has_outdoor:
            pass

        if has_co2:
            pass

class Agent:
  def __init__(self):
    self.dashboard = HugoDashBoard(config_path, HugoLogger(False))

    self.device_map = {
        "teset_device": {
          0x10: "test_device"
        }
    }

    self.message_ids = {}
    self.log_queue = Queue()

  async def notification_handler(self, sender, data):
    """
    Callback function that runs whenever a notification is received.
    """
    print(f"[{sender}] Notification received: {data.hex()}")

    header = data[:3]
    raw_sender_id = data[3:5]
    sender_id = int.from_bytes(raw_sender_id, "big")
    raw_receiver_id = data[5:7]
    receiver_id = int.from_bytes(raw_receiver_id, "big")
    raw_flag = data[7]
    flag = FlagByte(
        raw_flag & 0b10000000 != 0,
        raw_flag & 0b01000000 != 0,
        (raw_flag & 0b00110000) >> 4,
        raw_flag & 0b000011111,
    )
    message = data[8:-1]
    received_checksum = data[-1]
    calculated_checksum = raw_flag
    for byte in message:
        calculated_checksum ^= byte


    if (calculated_checksum != received_checksum):
        return

    device_name = self.device_map[sender_id] if sender_id in self.device_map else "unknown"

    print(f"header:{header.hex()}, sender_id:{hex(sender_id)}, receiver_id:{hex(receiver_id)}, flag:{flag}, message:{message}, rec_checksum:{hex(received_checksum)}, calc_chcksum:{hex(calculated_checksum)}")

    if flag.response_required:
        response_payload = b"\xf1\x01\x10" + raw_sender_id + b"\xaa\xb1\xe7\x01\x01"
        print(f"Sending response: {response_payload.hex()}")
        await global_client.write_gatt_char(NOTIFY_CHARACTERISTIC_UUID, response_payload, response=False)

    if sender not in self.message_ids or self.message_ids[sender] != flag.message_id:
        self.message_ids[sender] = flag.message_id

        if sender_id == 0x10:
            meteo = Meteo(self.log_queue, "text_meteo", message)
            meteo.publish(self.dashboard, False, False)


  async def main(self, address):
      global global_client
      print(f"Connecting to {address}...")

      async with BleakClient(address) as client:
          global_client = client
          if client.is_connected:
              print("Connected! Starting notifications...")

              # Start listening
              await client.start_notify(NOTIFY_CHARACTERISTIC_UUID, self.notification_handler)

              # Keep the script running while waiting for notifications
              print("Waiting for data (Press Ctrl+C to stop)...")
              try:
                  while True:
                      if not self.log_queue.empty():
                        self.dashboard.add_log(self.log_queue.get())

                      await asyncio.sleep(0.1)
              except asyncio.CancelledError:
                  await client.stop_notify(NOTIFY_CHARACTERISTIC_UUID)


agent = Agent()

try:
    asyncio.run(agent.main(DEVICE_ADDRESS))
except KeyboardInterrupt:
    print("\nStopped by user.")
