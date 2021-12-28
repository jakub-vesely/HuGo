import ___planner
from ___logging import Logging
#from ___rgb_led_block import RgbLedBlock
#from ___power_block import PowerBlock
#from ___motor_driver_block import MotorDriverBlock
#from ___button_block import ButtonBlock
#from ___chassis import Chassis

def pressed_q(logging , message):
  logging.info(message)
  #main_block.ble.get_keyboard().del_callback("q", pressed_q)

def pressed_r(rgb):
  rgb.toggle()

logging = Logging("events")

# def charging_state_changed(power_block):
#   logging.info("new charging state %d", power_block.is_charging.get_value())

# def voltage_changed(power_block):
#   logging.info("voltage changed: %.2f", power_block.battery_voltage_V.get_value())

def current_changed(power_block):
  logging.info("current changed: %.2f", power_block.battery_current_mA.get_value())

def sensor_changed_1(driver):
  logging.info("m1: %d", driver.motor1_sensor_counter.get_value())

def sensor_changed_2(driver):
  logging.info("m2: %d", driver.motor2_sensor_counter.get_value())

#toggle = False
#chassis = Chassis(None, 0x11, 0x12)

def button_pressed(button:ButtonBlock, rgb:RgbLedBlock):
  global toggle
  #logging.info("button: %d", button.is_pressed.get_value())
  rgb.set_on() if button.is_pressed.get_value() else rgb.set_off()

  if button.is_pressed.get_value():
    toggle = not toggle

    if toggle:
      chassis.go_forward(100)
      chassis.turn_right(10)
    else:
      chassis.stop()

def plan():
  # testing logging
  logging.warning("testing log")

  # testing virtual keyboard
  #$___main_block.ble.get_keyboard().add_callback("q", pressed_q, logging, message="ahoj")

  #rgb = RgbLedBlock()
  #___main_block.ble.get_keyboard().add_callback("r", pressed_r, rgb)

  # rgb = RgbLedBlock(0x02)
  # ___planner.repeat(1, rgb.toggle)

  #button = ButtonBlock(measurement_period=0.01)
  #button.is_pressed.changed(True, button_pressed, button, rgb)

  logging.info("done")
plan()





