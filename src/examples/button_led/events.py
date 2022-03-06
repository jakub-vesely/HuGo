#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from blocks.button_block import ButtonBlock
from blocks.rgb_led_block import RgbLedBlock

from basal.logging import Logging

class Plan:
  def __init__(self) -> None:
    self.button = ButtonBlock(measurement_period=0.1)
    self.led = RgbLedBlock()
    self.logging = Logging()
    self.button.value.changed(True, self._button_state_changed)

  def _button_state_changed(self):
    if self.button.value.get():
      self.led.set_on()
      self.logging.info("button pressed")
    else:
      self.led.set_off()
      self.logging.info("button released")

Plan()
