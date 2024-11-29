#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from basal.planner import Planner
from blocks.five_way_switch_block import FiveWaySwitchBlock

class FiweWaySwitch():
  logging = Logging("events")
  def __init__(self) -> None:
    self.block = FiveWaySwitchBlock()
    self.block.value.updated(self.updated)
    Planner().repeat(0.5, self.get_raw_value)

  def get_raw_value(self):
    self.logging.info(self.block.get_button_raw_value())
  def updated(self):
    value = self.block.value.get();
    if value != FiveWaySwitchBlock.button_none:
      self.logging.info(self.block.value.get())


FiweWaySwitch()
