#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.ble import Ble
from basal.logging import Logging
from basal.planner import Planner
from compilations.chassis import Chassis, Speed, Manoeuver, Direction
from remote_control.remote_key import RemoteKey

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x20, 0x21)

    Ble.value_remote.equal_to(RemoteKey("a"), self.turn_left)
    Ble.value_remote.equal_to(RemoteKey("d"), self.turn_right)
    Ble.value_remote.equal_to(RemoteKey("w"), self.speed_up)
    Ble.value_remote.equal_to(RemoteKey("s"), self.slow_down)
    Ble.value_remote.equal_to(RemoteKey("z"), self.stop)
    Ble.value_remote.equal_to(RemoteKey("x"), self.reverse)

    Planner.repeat(1, self.print_power_info)

  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage.get()
    current = self.chassis.power.battery_current_mA.get()
    self.logging.info("battery voltage: {0}, current: {1}".format(voltage, current))

  def slow_down(self):
    self.chassis.set_speed(self.chassis.speed - 1)

  def speed_up(self):
    self.chassis.set_speed(self.chassis.speed + 1)

  def turn_left(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver - 1)

  def turn_right(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver + 1)

  def pressed_l(self):
    self.chassis.set_direction(self.chassis.direction - 1)

  def reverse(self):
    self.logging.info(self.chassis.direction == Direction.forward)
    self.chassis.set_direction(Direction.backward if self.chassis.direction == Direction.forward else Direction.forward)

  def stop(self):
    self.chassis.set_speed(Speed.stop)
    self.chassis.set_manoeuver(Manoeuver.straight)

Plan()
