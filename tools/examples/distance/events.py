from logging import Logging
from planner import Planner
from distance_block import DistanceBlock
from display_block import DisplayBlock
from smoothed_variable import SmoothedVariable, SmoothingType
from rgb_led_block import RgbLedBlock, RgbLedBlockColor

class Plan():
  short = 200
  long = 400
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.display = DisplayBlock()
    self.rgb = RgbLedBlock()
    self.rgb.set_on()

    self.distance_block = DistanceBlock(measurement_period=0.05)
    self.smooth_distance = SmoothedVariable(3, SmoothingType.progressive, self.distance_block.distance)
    self.smooth_distance.less_than(self.short, False, self.short_distance)

    self.power_save_state = 0

    Planner.repeat(0.5, self.get_distance)

  def short_distance(self):
    self.logging.info("short_distance")
    self.rgb.set_color(RgbLedBlockColor.red)
    self.smooth_distance.more_than(self.long, False, self.long_distance)

  def long_distance(self):
    self.logging.info("long_distance")
    self.rgb.set_color(RgbLedBlockColor.green)
    self.smooth_distance.less_than(self.short, False, self.short_distance)

  def get_distance(self):
    raw_distance = self.distance_block.distance.get_value(False)
    self.display.clean()
    self.display.print_text(0, 0, str(raw_distance))
    self.display.print_text(0, 9, str(int(self.smooth_distance.get_value())))
    self.display.showtime()
Plan()