#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.logging import Logging
from blocks.power_block import PowerBlock
from blocks.display_block import DisplayBlock

class PowerMonitor():
  logging = Logging("events")
  def __init__(self) -> None:
    self.power = PowerBlock()
    self.display = DisplayBlock()

    self.power.is_usb_connected.changed(True, self.usb_state_changed)
    self.power.is_charging.changed(True, self.charging_state_changed)
    self.power.battery_voltage_V.changed(True, self.voltage_changed)
    self.power.battery_current_mA.changed(True, self.current_changed)

    self.usb_state_changed()
    self.charging_state_changed()
    self.voltage_changed()
    self.current_changed()

  def display_line(self, x0, y0, text):
    self.display.fill_rect(x0, y0, self.display.get_dimensions()[0], 8, 0)
    self.display.print_text(x0, y0, text)
    self.display.showtime()


  def usb_state_changed(self):
    usb  =  self.power.is_usb_connected.get()
    self.logging.info("new usb state %d", usb)
    self.display_line(0, 2, "USB: {}".format("Yes" if usb else "No") )

  def charging_state_changed(self):
    charging  =  self.power.is_charging.get()
    self.logging.info("new charging state %d", charging)
    self.display_line(0, 13, "char:{}".format("Yes" if charging else "No") )

  def voltage_changed(self):
    voltage = self.power.battery_voltage_V.get()
    self.logging.info("voltage changed: %.3f", voltage)
    self.display_line(0, 24, "{:.3f} V".format(voltage))

  def current_changed(self):
    current = self.power.battery_current_mA.get()
    self.logging.info("current changed: %.1f", current)
    self.display_line(0, 35, "{:.1f} mA".format(current))

PowerMonitor()
