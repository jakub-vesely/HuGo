from logging import Logging
from planner import Planner
from display_block import DisplayBlock
from power_mgmt import PowerMgmt, PowerPlan
from rgb_led_block import RgbLedBlock, RgbLedBlockColor
from ble import Ble
from chassis import Chassis, Speed, Manoeuver, Direction
from smoothed_variable import SmoothedVariable, SmoothingType
from distance_block import DistanceBlock
from button_block import ButtonBlock

class Plan():
  near_barrier = 300
  far_barrier = 600
  open_space = 900
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(0x11, 0x12, None, 0.3)
    self.rgb = RgbLedBlock()
    self.display = DisplayBlock()
    self.distance = DistanceBlock(measurement_period=0.1)
    self.button = ButtonBlock(measurement_period=0.1)

    self.button.value.equal_to(True, True, self.change_patrol)

    self.near_barrier_event = None
    self.far_bariier_event = None
    self.enough_space_event = None
    self.open_space_event = None
    self.patrol = False

    self.display.contrast(0)
    keyboard = Ble.get_keyboard()
    keyboard.add_callback("a", self.turn_left)
    keyboard.add_callback("d", self.turn_right)
    keyboard.add_callback("w", self.speed_up)
    keyboard.add_callback("s", self.slow_down)
    keyboard.add_callback("z", self.stop)
    keyboard.add_callback("x", self.reverse)
    keyboard.add_callback("p", self.change_patrol)

    self.counter = 0
    Planner.repeat(0.5, self.print_power_info)
    PowerMgmt.set_plan(PowerPlan.get_max_performance_plan())

    self.current_smoother = SmoothedVariable(3, SmoothingType.average, self.chassis.power.battery_current_mA)
    self.current_smoother.more_than(600, True, self.stop_for_a_while, 1)
    self.heart_beat = False

  def define_patrol_events(self):
    self.near_barrier_event = self.distance.value.less_than(self.near_barrier, True, self.near_barrier_detected)
    self.far_bariier_event = self.distance.value.in_range(self.near_barrier, self.far_barrier, True, self.far_barrier_detected)
    self.enough_space_event = self.distance.value.in_range(self.far_barrier, self.open_space, True, self.enough_space_detected)
    self.open_space_event = self.distance.value.more_than(self.open_space - 1, True, self.open_space_detected)

  def cancel_patrol_events(self):
    self.distance.value.remove_trigger(self.near_barrier_event)
    self.distance.value.remove_trigger(self.far_bariier_event)
    self.distance.value.remove_trigger(self.enough_space_event)
    self.distance.value.remove_trigger(self.open_space_event)

  def near_barrier_detected(self):
    self.logging.info("near_barrier_detected")
    self.rgb.set_color(RgbLedBlockColor.red)
    self.chassis.set_speed(Speed.slow)
    self.chassis.set_direction(Direction.backward)
    self.chassis.set_manoeuver(Manoeuver.sharply_right) #with reverse will turn left

  def far_barrier_detected(self):
    self.logging.info("far_barrier_detected")
    self.rgb.set_color(RgbLedBlockColor.orange)
    self.chassis.set_speed(Speed.slow)
    self.chassis.set_direction(Direction.forward)
    self.chassis.set_manoeuver(Manoeuver.straight)

  def enough_space_detected(self):
    self.logging.info("enough_space")
    self.rgb.set_color(RgbLedBlockColor.blue)
    self.chassis.set_speed(Speed.normal)
    self.chassis.set_direction(Direction.forward)
    self.chassis.set_manoeuver(Manoeuver.straight)

  def open_space_detected(self):
    self.logging.info("open_space")
    self.rgb.set_color(RgbLedBlockColor.green)
    self.chassis.set_speed(Speed.normal)
    self.chassis.set_direction(Direction.forward)
    self.chassis.set_manoeuver(Manoeuver.slightly_right) #can rotate another wat

  def change_patrol(self):
    if self.patrol:
      self.logging.info("patrol mode canceled")
      self.cancel_patrol_events()
      self.chassis.set_speed(Speed.stop)
    else:
      self.logging.info("patrol mode activated")
      self.define_patrol_events()
      self.chassis.set_speed(Speed.slow)
    self.patrol = not self.patrol


  def print_power_info(self):
    voltage = self.chassis.power.battery_voltage_V.get_value()
    current = self.chassis.power.battery_current_mA.get_value()

    self.display.clean()
    self.heart_beat = not self.heart_beat
    if self.heart_beat:
      self.display.print_text(55, 0, "*")
    self.display.print_text(0, 9, "%2.3f V" % voltage)

    self.display.print_text(0, 18, "%3.2f mA" % self.current_smoother.get_value())
    self.display.print_text(0, 36, str(self.distance.value.get_value()))
    self.counter += 1
    self.display.showtime()

  def slow_down(self):
    self.chassis.set_speed(self.chassis.speed - 1)

  def speed_up(self):
    self.chassis.set_speed(self.chassis.speed + 1)

  def turn_left(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver - 1)

  def turn_right(self):
    self.chassis.set_manoeuver(self.chassis.manoeuver + 1)

  def reverse(self):
    self.logging.info(self.chassis.direction == Direction.forward)
    self.chassis.set_direction(Direction.backward if self.chassis.direction == Direction.forward else Direction.forward)

  def stop_for_a_while(self, delay):
    self.stop() #take a rest
    Planner.postpone(delay, self.chassis.set_speed, Speed.slow) #... and slowly continue

  def stop(self):
    self.chassis.set_speed(Speed.stop)
    self.chassis.set_manoeuver(Manoeuver.straight)

Plan()