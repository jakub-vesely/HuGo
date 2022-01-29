from power_block import PowerBlock
from logging import Logging
logging = Logging("events")

def charging_state_changed(power_block):
  logging.info("new charging state %d", power_block.is_charging.get_value())

def voltage_changed(power_block):
  logging.info("voltage changed: %.2f", power_block.battery_voltage_V.get_value())

def current_changed(power_block):
  logging.info("current changed: %.2f", power_block.battery_current_mA.get_value())

def plan():
  power = PowerBlock(0x02)
  power.is_charging.changed_repeat(charging_state_changed, power)
  power.battery_voltage_V.changed_repeat(voltage_changed, power)
  power.battery_current_mA.changed_repeat(current_changed, power)

plan()