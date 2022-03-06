#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from blocks.power_block import PowerBlock
from basal.logging import Logging

logging = Logging("events")

def charging_state_changed(power_block):
  logging.info("new charging state %d", power_block.is_charging.get())

def voltage_changed(power_block):
  logging.info("voltage changed: %.2f", power_block.battery_voltage_V.get())

def current_changed(power_block):
  logging.info("current changed: %.2f", power_block.battery_current_mA.get())

def plan():
  power = PowerBlock()
  power.is_charging.changed(True, charging_state_changed, power)
  power.battery_voltage_V.changed(True, voltage_changed, power)
  power.battery_current_mA.changed(True, current_changed, power)

plan()
