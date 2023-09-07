#  Copyright (c) 2023 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from basal.planner import Planner
from blocks.ble_block import BleBlock
from blocks.block_base import PowerSaveLevel
from basal.remote_value import RemoteValue
from quantities.temperature import Temperature
from quantities.pressure import Pressure
from quantities.humidity import RelativeHumidity
from quantities.current import Current
from quantities.voltage import Voltage
from basal.active_variable import ActiveVariable
from basal.active_quantity import ActiveQuantity

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.ble = BleBlock()
    self.quantities = []

    temperature = ActiveQuantity(Temperature("t", 3))
    RemoteValue.add("test.meteo.temperature", temperature)
    self.quantities.append(temperature)

    pressure = ActiveQuantity(Pressure("P", 3))
    RemoteValue.add("test.meteo.pressure", pressure)
    self.quantities.append(pressure)

    humidity = ActiveQuantity(RelativeHumidity("RH", 3))
    RemoteValue.add("test.meteo.pressure", humidity)
    self.quantities.append(humidity)

    current = ActiveQuantity(Current("I", 3))
    RemoteValue.add("test.power.battery_current", current)
    self.quantities.append(current)

    voltage = ActiveQuantity(Voltage("U", 3))
    RemoteValue.add("test.power.battery_voltage", voltage)
    self.quantities.append(voltage)

    self.is_usb_connected = ActiveVariable(False)
    self.is_charging = ActiveVariable(False)
    self.heartbeat = ActiveVariable(False)

    RemoteValue.add("test.power.is_usb_connected", self.is_usb_connected)
    RemoteValue.add("test.power.is_charging", self.is_charging)

    RemoteValue.add("test.main.status", self.heartbeat)

    #self.event = Planner.postpone(1, self.ble.sleep)
    #while True:
      #response = self.ble.read_at_command_response(0.1)
      #if not response:
      #  break
      #self.logging.info(response)

    #self.logging.info("go to sleep");

    #self.event = Planner.postpone(1, self.sleep)
    #self.event = Planner.repeat(0.5, self.read)
    self.event = Planner.repeat(0.3, self.read_mesh)
    #Planner.repeat(5, self.send_mesh)
    #Planner.postpone(1, self.write)


  def write(self):
    #self.ble.write_at_command(b"AT\x0d\x0a")
    #self.ble.write_at_command("AT+NAMEJDY-03")
    #self.ble.write_at_command("AT+ROLE5")
    #self.ble.write_at_command("AT+MCLSS0")
    #self.ble.write_at_command("AT+RESET")
    #self.ble.write_at_command("AT+SLEEP2")
    #self.ble.write_at_command("W")
    #self.ble.write_at_command("AT+NETID1122")
    #self.ble.write_at_command(b"AT+MADDR\x0d\x0a")
    #self.ble.write_at_command("AT+MADDR0002")
    #self.ble.write_at_command(bytes.fromhex("41542b4d45534801000241430d0a"))
    self.ble.write_at_command(b"AT+MESH\x01\x00\x03I=200 mA\x0d\x0a\x0d\x0a")

  def send_mesh(self):
     #self.ble.write_at_command(b"AT+MADDR\x0d\x0a")
     self.ble.write_at_command(b"AT+MESH\x00\x00\x03I=200 mA\x0d\x0a")
     #resp = self.ble.send_mesh_message(0x03, b":-)")
     #self.logging.info(resp)

  def wakeup(self):
    self.ble.power_save(PowerSaveLevel.NoPowerSave)

  def sleep(self):
    self.ble.power_save(PowerSaveLevel.DeepPowerSave)
    Planner.postpone(1, self.wakeup)

  def read(self):
    data = self.ble.read_at_command_response(0.3)
    if data:
      self.logging.info(data)

  def read_mesh(self):
    while (True):
      data = self.ble.read_mesh_message()
      if not data:
        break;
      device_id = data[0];
      message:str = data[1:].decode('ascii')
      parts = message.split("=")
      quantity_name = parts[0]
      parts = parts[1].split(" ")
      value = float(parts[0])
      units = parts[1] if len(parts) > 1 else ""

      if device_id == 3:
        quantity: ActiveQuantity
        for quantity in self.quantities:
          if quantity.get_name() == quantity_name:
            quantity.set_with_units(value, units)
            return
        if quantity_name == "usb":
          self.is_usb_connected.set(value)
        elif quantity_name == "charg":
          self.is_charging.set(value)
        elif quantity_name == "heart":
          self.heartbeat.set(value)
        else:
          self.logging.value(data)

Plan()
