#  Copyright (c) 2023 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from basal.planner import Planner
from blocks.ble_block import BleBlock
from blocks.main_block import MainBlock
from blocks.block_base import PowerSaveLevel
from basal.remote_value import RemoteValue
from quantities.temperature import Temperature
from quantities.pressure import Pressure
from quantities.humidity import RelativeHumidity
from quantities.angle import Angle
from quantities.speed import Speed
from quantities.length import length
from quantities.current import Current
from quantities.voltage import Voltage
from basal.active_variable import ActiveVariable
from basal.active_quantity import ActiveQuantity
from basal.power_mgmt import PowerMgmt, PowerPlan

class DeviceQuantity(ActiveQuantity):
  def __init__(self, device_id:int, quantity:str):
    super().__init__(quantity)
    self._device_id = device_id
class DeviceVariable(ActiveVariable):
  def __init__(self, device_id:int, name:str, initial_value=None):
    super().__init__(initial_value)
    self._name = name
    self._device_id = device_id

  def get_name(self):
    return self._name

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.ble = BleBlock()
    self.active_vars = []

    PowerMgmt.set_plan(PowerPlan.get_max_performance_plan())

    mainBlock = MainBlock();

    #reboot once a day to clean fragmented RAM
    Planner.postpone(60 * 60 * 24, mainBlock.reboot)

    self.add_variable(DeviceQuantity(5, Temperature("t", 3)), "in1.ambient.temperature")
    self.add_variable(DeviceQuantity(5, Pressure("P", 3)), "in1.ambient.pressure")
    self.add_variable(DeviceQuantity(5, RelativeHumidity("RH", 3)), "in1.ambient.humidity")

    self.add_variable(DeviceQuantity(5, Current("I", 3)), "in1.power.battery_current")
    self.add_variable(DeviceQuantity(5, Voltage("U", 3)), "in1.power.battery_voltage")

    self.add_variable(DeviceQuantity(3, Temperature("t", 3)), "test.ambient.temperature")
    self.add_variable(DeviceQuantity(3, Pressure("P", 3)), "test.ambient.pressure")
    self.add_variable(DeviceQuantity(3, RelativeHumidity("RH", 3)), "test.ambient.humidity")

    self.add_variable(DeviceQuantity(3, Angle("wd", 3)), "test.ambient.wind_direction")
    self.add_variable(DeviceQuantity(3, Speed("ws", 3)), "test.ambient.wind_speed")
    self.add_variable(DeviceQuantity(3, length("rain", 3)), "test.ambient.rain_gauge")

    self.add_variable(DeviceQuantity(3, Current("I", 3)), "test.power.battery_current")
    self.add_variable(DeviceQuantity(3, Voltage("U", 3)), "test.power.battery_voltage")

    self.add_variable(DeviceVariable(3, "usb", False), "test.power.is_usb_connected")
    self.add_variable(DeviceVariable(3, "charg", False), "test.power.is_charging")
    self.add_variable(DeviceVariable(3, "heart", False), "test.main.status")

    #from blocks.rj12 import Rj12Block
    #rj_block = Rj12Block(0x30)
    #rj_block.change_block_address(0x30)
    #self.logging.info(rj_block.is_available())

    #self.event = Planner.postpone(1, self.ble.sleep)
    #while True:
      #response = self.ble.read_at_command_response(0.1)
      #if not response:
      #  break
      #self.logging.info(response)

    #self.logging.info("go to sleep");

    #self.event = Planner.postpone(1, self.sleep)
    #self.event = Planner.repeat(0.5, self.read)
    self.event = Planner.repeat(0.05, self.read_mesh)
    #Planner.repeat(5, self.send_mesh)
    #Planner.postpone(1, self.write)


  def add_variable(self, quantity, remote_id):
    RemoteValue.add(remote_id, quantity)
    self.active_vars.append(quantity)

  #def write(self):
    #self.ble.write_at_command(b"AT") #communication test
    #self.ble.write_at_command("AT+NAMEJDY-04") #set name
    #self.ble.write_at_command("AT+ROLE5") #set role
    #self.ble.write_at_command("AT+MCLSS1") #0:router, 1: terminal
    #self.ble.write_at_command("AT+RESET")
    #self.ble.write_at_command("AT+SLEEP2") #1: light, 2:deep
    #self.ble.write_at_command("W")
    #self.ble.write_at_command("AT+NETID1122") #id of network
    #self.ble.write_at_command(b"AT+MADDR\x0d\x0a") # get addr
    #self.ble.write_at_command("AT+MADDR0005") #set addr
    #self.ble.write_at_command(bytes.fromhex("41542b4d45534801000241430d0a"))

    #self.ble.write_at_command(b"AT+MESH\x01\x00\x03I=200 mA\x0d\x0a\x0d\x0a")

  #def send_mesh(self):
     #self.ble.write_at_command(b"AT+MADDR\x0d\x0a")
     #self.ble.write_at_command(b"AT+MESH\x00\x00\x03I=200 mA\x0d\x0a")
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
      #self.logging.info(data)

      device_id = data[0];
      message:str = data[1:].decode('ascii')
      parts = message.split("=")
      var_name = parts[0]
      parts = parts[1].split(" ")
      value = float(parts[0])
      units = parts[1] if len(parts) > 1 else ""

      for variable in self.active_vars:
        if variable._device_id == device_id and variable.get_name() == var_name:
          if isinstance(variable, DeviceQuantity):
            variable.set_with_units(value, units)
          else:
            variable.set(value)
      #self.logging.value(data)

Plan()
