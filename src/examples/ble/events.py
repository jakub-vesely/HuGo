#  Copyright (c) 2023 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from basal.planner import Planner
from blocks.ble_block import BleBlock
from blocks.block_base import PowerSaveLevel

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.ble = BleBlock()
    #self.event = Planner.postpone(1, self.ble.sleep)
    while True:
      response = self.ble.read_at_command_response(0.1)
      if not response:
        break
      self.logging.info(response)

    #self.logging.info("go to sleep");

    #self.event = Planner.postpone(1, self.sleep)
    #self.event = Planner.repeat(0.2, self.read)
    self.event = Planner.repeat(0.2, self.read_mesh)
    Planner.repeat(2, self.send_mesh)
    #Planner.postpone(1, self.write)


  def write(self):
    self.ble.write_at_command("AT")
    #self.ble.write_at_command("AT+NAMEJDY-03")
    #self.ble.write_at_command("AT+ROLE5")
    #self.ble.write_at_command("AT+MCLSS0")
    #self.ble.write_at_command("AT+RESET")
    #self.ble.write_at_command("AT+SLEEP2")
    #self.ble.write_at_command("W")
    #self.ble.write_at_command("AT+NETID1122")
    #self.ble.write_at_command("AT+MADDR0002")
    #self.ble.write_at_command(bytes.fromhex("41542b4d45534801000241430d0a"))
    #self.ble.write_at_command(b"AT+MESH\x01\x00\x02ahoj\x0d\x0a")

  def send_mesh(self):
     resp = self.ble.send_mesh_message(0x02, b":-)")
     self.logging.info(resp)

  def wakeup(self):
    self.ble.power_save(PowerSaveLevel.NoPowerSave)

  def sleep(self):
    self.ble.power_save(PowerSaveLevel.DeepPowerSave)
    Planner.postpone(1, self.wakeup)

  def read(self):
    data = self.ble.read_at_command_response()
    if data:
      self.logging.info(data)

  def read_mesh(self):
    data = self.ble.read_mesh_message(0.3)
    if data:
      self.logging.info(data)

Plan()
