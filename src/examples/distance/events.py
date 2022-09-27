#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT
from basal.logging import Logging
from basal.planner import Planner
from basal.smoothed_variable import SmoothedVariable, SmoothingType
from blocks.distance_block import DistanceBlock
from blocks.display_block import DisplayBlock
from blocks.rgb_led_block import RgbLedBlock, RgbLedBlockColor

class Plan():
  border_distance = 200
  def __init__(self) -> None:
    self.logging = Logging("events")
    self.display = DisplayBlock()
    self.rgb = RgbLedBlock()
    self.rgb.set_on()

    self.distance_block = DistanceBlock(measurement_period=0.05)
    self.smooth_distance = SmoothedVariable(3, SmoothingType.progressive, self.distance_block.value)
    self.short_distance_event = self.smooth_distance.less_than(self.border_distance, self.short_distance)
    self.smooth_distance.more_than(self.border_distance, self.long_distance)
    self.power_save_state = 0

    Planner.repeat(0.5, self.get_distance)

  def short_distance(self):
    self.logging.info("short_distance")
    self.rgb.set_color(RgbLedBlockColor.red)

  def long_distance(self):
    self.logging.info("long_distance")
    self.rgb.set_color(RgbLedBlockColor.green)

  def get_distance(self):
    raw_distance = self.distance_block.value.get(False)
    self.display.clean()
    self.display.print_text(0, 0, str(raw_distance))
    self.display.print_text(0, 9, str(int(self.smooth_distance.get())))
    self.display.showtime()

Plan()
