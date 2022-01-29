from chassis import Chassis, Speed, Manoeuver, Direction
from main_block import MainBlock
from logging import Logging
from planner import Planner

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None)
    MainBlock.ble.get_keyboard().add_callback("a", self.pressed_a)
    MainBlock.ble.get_keyboard().add_callback("d", self.pressed_d)
    MainBlock.ble.get_keyboard().add_callback("w", self.pressed_w)
    MainBlock.ble.get_keyboard().add_callback("s", self.pressed_s)
    MainBlock.ble.get_keyboard().add_callback("z", self.pressed_z)
    MainBlock.ble.get_keyboard().add_callback("x", self.pressed_x)

    Planner.repeat(1, self.print_power_info)

  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage_V.get_value()
    current = self.chassis.power.battery_current_mA.get_value()
    self.logging.info("battery voltage: {0}, current: {1}".format(voltage, current))

  def pressed_s(self):
    self.chassis.set_speed(self.chassis.speed - 1)

  def pressed_w(self):
    self.chassis.set_speed(self.chassis.speed + 1)

  def pressed_a(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver - 1)

  def pressed_d(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver + 1)

  def pressed_l(self):
    self.chassis.set_direction(self.chassis.direction - 1)

  def pressed_x(self):
    self.logging.info(self.chassis.direction == Direction.forward)
    self.chassis.set_direction(Direction.backward if self.chassis.direction == Direction.forward else Direction.forward)

  def pressed_z(self):
    self.chassis.set_speed(Speed.stop)
    self.chassis.set_manoeuver(Manoeuver.straight)

Plan()