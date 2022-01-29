from logging import Logging
from planner import Planner
from display_block import DisplayBlock
from power_mgmt import PowerMgmt, PowerPlan
from ble import Ble
from chassis import Chassis, Speed, Manoeuver, Direction
from smoother import Smoother, SmoothingMethod

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None)
    self.display = DisplayBlock()
    self.display.contrast(0)
    keyboard = Ble.get_keyboard()
    keyboard.add_callback("a", self.pressed_a)
    keyboard.add_callback("d", self.pressed_d)
    keyboard.add_callback("w", self.pressed_w)
    keyboard.add_callback("s", self.pressed_s)
    keyboard.add_callback("z", self.pressed_z)
    keyboard.add_callback("x", self.pressed_x)

    self.counter = 0
    Planner.repeat(0.5, self.print_power_info)
    PowerMgmt.set_plan(PowerPlan.get_balanced_plan())

    self.current_smoother = Smoother(4, SmoothingMethod.progressive)
    self.heart_beat = False

  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage_V.get_value()
    current = self.chassis.power.battery_current_mA.get_value()
    self.current_smoother.add(current)
    smooth_current = self.current_smoother.get()
    if smooth_current > 500:
      self.pressed_s() #slow down

    #ps_active = Ble.power_save_active.get_value()
    timer = 0 #Ble.time_to_power_save
    self.display.clean()
    self.heart_beat = not self.heart_beat
    if self.heart_beat:
      self.display.print_text(55, 0, "*")
    self.display.print_text(0, 9, "%2.3f V" % voltage)

    self.display.print_text(0, 18, "%3.2f mA" % smooth_current)
    self.display.print_text(0, 27, str(timer))
    self.display.print_text(0, 36, str(self.counter))
    self.counter += 1
    self.display.showtime()

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