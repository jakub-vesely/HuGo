import ___planner
from ___logging import Logging
from rgb_led_block import RgbLedBlock
from ___display_block import DisplayBlock
from power_block import PowerBlock
def print_message(mesage):
  print(mesage)

def pressed_q(logging , message):
  logging.info(message)
  #main_block.ble.get_keyboard().del_callback("q", pressed_q)

def pressed_r(rgb):
  rgb.toggle()

logging = Logging("events")

def charging_state_changed(power_block):
  logging.info("new charging state %d", power_block.is_charging.get_value())

def voltage_changed(power_block):
  logging.info("voltage changed: %.2f", power_block.battery_voltage_V.get_value())

def current_changed(power_block):
  logging.info("current changed: %.2f", power_block.battery_current_mA.get_value())

def plan():
  # testing planner
  #___planner.postpone(0.01, print_message, "Hello World")

  # testing logging
  logging.warning("testing log")

  # testing virtual keyboard
  #$___main_block.ble.get_keyboard().add_callback("q", pressed_q, logging, message="ahoj")

  #rgb = RgbLedBlock(0x02)
  #___main_block.ble.get_keyboard().add_callback("r", pressed_r, rgb)

  #display = DisplayBlock(0x05)
  #display.change_block_address(0x05)
  #display.print_text(0,0, " ahoj")
  #display.showtime()
  #logging.info(str(("ext_count", display.get_extension_count())))
  # logging.info(str(("ext_address_list", display.get_ext_address_list())))
  # logging.info(str(("ext_address", display.get_extension_address())))
  # logging.info(str(("ext_address_change", display.change_extension_address(0x3c))))

  power = PowerBlock(0x02)
  power.is_charging.changed_repeat(charging_state_changed, power)
  power.battery_voltage_V.changed_repeat(voltage_changed, power)
  power.battery_current_mA.changed_repeat(current_changed, power)

  #logging.info("version", power.block_version)


  #power.change_block_address(0x04)
  # logging.info(str(("ext_count", power.get_extension_count())))
  # logging.info(str(("ext_address_list", power.get_ext_address_list())))
  # logging.info(str(("ext_address", power.get_extension_address())))

  # logging.info(str(("usb", power.is_usb_connected.get_value())))
  # logging.info("a")
  #logging.info(str(("charging", power.is_charging.get_value())))
  #logging.info("b")
plan()





