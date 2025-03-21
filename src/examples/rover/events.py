from basal.ble import Ble
from basal.logging import Logging
from basal.planner import Planner
from basal.power_mgmt import PowerMgmt, PowerPlan
from basal.smoothed_variable import SmoothedVariable, SmoothingType
from blocks.display_block import DisplayBlock
from blocks.rgb_led_block import RgbLedBlock, RgbLedBlockColor
from blocks.distance_block import DistanceBlock
from blocks.button_block import ButtonBlock
from compilations.chassis import Chassis, Speed, Manoeuver, Direction
from blocks.ir_block import IrBlock
from remote_control.ir_numeric_remote import IrNumericRemote
from remote_control.remote_key import RemoteKey
#from white_ir_rc import WhiteIrRc

class Plan():
  near_barrier = 300
  far_barrier = 600
  open_space = 900


  def __init__(self) -> None:
    self.logging = Logging("events")
    self.chassis = Chassis(power_measurement_period=0.3)
    self.rgb = RgbLedBlock()
    self.display = DisplayBlock()
    self.distance = DistanceBlock(measurement_period=0.1)
    self.button = ButtonBlock(measurement_period=0.1)
    self.ir_block = IrBlock()
    self.ir_block.add_remote(IrNumericRemote())

    self.button.value.equal_to(True, self.change_patrol)

    self.near_barrier_event = None
    self.far_bariier_event = None
    self.enough_space_event = None
    self.open_space_event = None
    self.patrol = False

    self.display.contrast(0)
    Ble.value_remote.equal_to(RemoteKey("a"), self.turn_left)
    Ble.value_remote.equal_to(RemoteKey("d"), self.turn_right)
    Ble.value_remote.equal_to(RemoteKey("w"), self.speed_up)
    Ble.value_remote.equal_to(RemoteKey("s"), self.slow_down)
    Ble.value_remote.equal_to(RemoteKey("z"), self.stop)
    Ble.value_remote.equal_to(RemoteKey("x"), self.reverse)
    Ble.value_remote.equal_to(RemoteKey("p"), self.change_patrol)

    self.ir_block.value.equal_to(IrNumericRemote.key_left, self.turn_left)
    self.ir_block.value.equal_to(IrNumericRemote.key_right, self.turn_right)
    self.ir_block.value.equal_to(IrNumericRemote.key_up, self.speed_up)
    self.ir_block.value.equal_to(IrNumericRemote.key_down, self.slow_down)
    self.ir_block.value.equal_to(IrNumericRemote.key_ok, self.stop)
    self.ir_block.value.equal_to(IrNumericRemote.key_hash, self.reverse)
    self.ir_block.value.equal_to(IrNumericRemote.key_star, self.change_patrol)

    self.counter = 0
    Planner.repeat(0.5, self.print_power_info)
    PowerMgmt.set_plan(PowerPlan.get_max_performance_plan())

    self.current_smoother = SmoothedVariable(3, SmoothingType.average, self.chassis.power.battery_current)
    self.current_smoother.more_than(600, self.stop_for_a_while, 1)
    self.heart_beat = False

  def define_patrol_events(self):
    self.near_barrier_event = self.distance.value.less_than(self.near_barrier, self.near_barrier_detected)
    self.far_bariier_event = self.distance.value.in_range(self.near_barrier, self.far_barrier, self.far_barrier_detected)
    self.enough_space_event = self.distance.value.in_range(self.far_barrier, self.open_space, self.enough_space_detected)
    self.open_space_event = self.distance.value.more_than(self.open_space - 1, self.open_space_detected)

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
    voltage_str = str(self.chassis.power.battery_voltage)

    self.display.clean()
    self.heart_beat = not self.heart_beat
    if self.heart_beat:
      self.display.print_text(55, 0, "*")
    self.display.print_text(0, 9, voltage_str)

    self.display.print_text(0, 18, "%3.2f mA" % self.current_smoother.get())
    self.display.print_text(0, 36, str(self.distance.value.get()))
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
