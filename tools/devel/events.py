from ___chassis import Chassis, Speed, Manoeuver, Direction
import ___main_block as main_block
from ___logging import Logging
import ___planner as planner
from ___display_block import DisplayBlock
from ___power_mgmt import PowerMgmt, PowerPlan, BlePowerPlan

class Plan():
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None)
    self.display = DisplayBlock()
    main_block.ble.get_keyboard().add_callback("a", self.pressed_a)
    main_block.ble.get_keyboard().add_callback("d", self.pressed_d)
    main_block.ble.get_keyboard().add_callback("w", self.pressed_w)
    main_block.ble.get_keyboard().add_callback("s", self.pressed_s)
    main_block.ble.get_keyboard().add_callback("z", self.pressed_z)
    main_block.ble.get_keyboard().add_callback("x", self.pressed_x)

    self.counter = 0
    planner.repeat(5, self.print_power_info)
    PowerMgmt.set_plan(PowerPlan(True, PowerPlan.freq_min, BlePowerPlan(15, 3, 10)))


  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage_V.get_value()
    current = self.chassis.power.battery_current_mA.get_value()
    #ps_active = main_block.ble.power_save_active.get_value()
    timer = main_block.ble.time_to_power_save
    self.display.clean()
    self.display.print_text(0, 9, str(voltage) + " V")
    self.display.print_text(0, 18, str(current) + " mA")
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