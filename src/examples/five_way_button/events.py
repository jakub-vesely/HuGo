#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from ___basal.___logging import Logging
from ___blocks.___five_way_switch_block import FiveWaySwitchBlock

class FiweWaySwitch():
  logging = Logging("events")
  def __init__(self) -> None:
    self.block = FiveWaySwitchBlock()
    self.block.value.updated(self.updated)

  def updated(self):
    self.logging.info(self.block.value.get())


FiweWaySwitch()
