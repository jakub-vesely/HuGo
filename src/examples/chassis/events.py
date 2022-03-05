from chassis import Chassis, Speed, Manoeuver, Direction
from virtual_keyboard import VirtualKeyboard
from logging import Logging
from planner import Planner
from ble import Ble

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None)

    VirtualKeyboard.add_callback("a", self.turn_left)
    VirtualKeyboard.add_callback("d", self.turn_right)
    VirtualKeyboard.add_callback("w", self.speed_up)
    VirtualKeyboard.add_callback("s", self.slow_down)
    VirtualKeyboard.add_callback("z", self.stop)
    VirtualKeyboard.add_callback("x", self.reverse)

    Planner.repeat(1, self.print_power_info)

  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage_V.get()
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