from chassis import Chassis, Speed, Manoeuver, Direction
import main_block as main_block
from logging import Logging
import planner as planner
class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None)
    main_block.ble.get_keyboard().add_callback("a", self.pressed_a)
    main_block.ble.get_keyboard().add_callback("d", self.pressed_d)
    main_block.ble.get_keyboard().add_callback("w", self.pressed_w)
    main_block.ble.get_keyboard().add_callback("s", self.pressed_s)
    main_block.ble.get_keyboard().add_callback("z", self.pressed_z)
    main_block.ble.get_keyboard().add_callback("x", self.pressed_x)

    planner.repeat(1, self.print_power_info)

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