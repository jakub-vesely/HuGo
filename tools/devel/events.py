import ___planner
from ___logging import Logging
from ___rgb_led_block import RgbLedBlock
#from ___display_block import DisplayBlock
#from ___power_block import PowerBlock
#from ___motor_driver_block import MotorDriverBlock
from ___button_block import ButtonBlock
from ___chassis import Chassis

def print_message(mesage):
  print(mesage)

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

toggle = False
chassis = Chassis(None, 0x11, 0x12)

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
  # testing planner
  #___planner.postpone(0.01, print_message, "Hello World")

  # testing logging
  logging.warning("testing log")

  # testing virtual keyboard
  #$___main_block.ble.get_keyboard().add_callback("q", pressed_q, logging, message="ahoj")

  rgb = RgbLedBlock()
  #___main_block.ble.get_keyboard().add_callback("r", pressed_r, rgb)

  #display = DisplayBlock(0x05)
  #display.change_block_address(0x05)
  #display.print_text(0,0, " ahoj")
  #display.showtime()
  #logging.info(str(("ext_count", display.get_extension_count())))
  # logging.info(str(("ext_address_list", display.get_ext_address_list())))
  # logging.info(str(("ext_address", display.get_extension_address())))
  # logging.info(str(("ext_address_change", display.change_extension_address(0x3c))))

  #power = PowerBlock()
  # power.is_charging.changed_repeat(charging_state_changed, power)
  # power.battery_voltage_V.changed_repeat(voltage_changed, power)
  #power.battery_current_mA.changed(True, current_changed, power)

  #logging.info("version", power.block_version)


  #power.change_block_address(0x08)
  # logging.info(str(("ext_count", power.get_extension_count())))
  # logging.info(str(("ext_address_list", power.get_ext_address_list())))
  # logging.info(str(("ext_address", power.get_extension_address())))

  # logging.info(str(("usb", power.is_usb_connected.get_value())))
  # logging.info("a")
  #logging.info(str(("charging", power.is_charging.get_value())))
  #logging.info("b")

  #motor_driver_f = MotorDriverBlock(0x11)
  #motor_driver_f.change_block_address(0x11)
  #motor_driver_r = MotorDriverBlock(0x12)
  #motor_driver_r.change_block_address(0x12)
  #motor_driver.turn_clockwise(motor_driver.motor1_id)

  #logging.info("counter", motor_driver.get_sensor_counter(motor_driver.motor1_id))
  # motor_driver_f.sensor_power_on()
  # motor_driver_f.motor1_sensor_counter.changed(True, sensor_changed_1, motor_driver_f)
  # motor_driver_f.motor2_sensor_counter.changed(True, sensor_changed_2, motor_driver_f)
  #motor_driver.turn_clockwise(motor_driver.motor1_id)
  #motor_driver_f.set_speed(motor_driver_f.motor1_id, 100)
  #motor_driver_f.set_speed(motor_driver_f.motor2_id, 100)
  #motor_driver_f.set_pwm_period(100)
  #motor_driver_r.set_speed(motor_driver_r.motor1_id, 100)
  #motor_driver_r.set_speed(motor_driver_r.motor2_id, 100)
  #motor_driver.turn_opposite(motor_driver.motor1_id)
  #motor_driver.turn_opposite(motor_driver.motor2_id)

  # rgb = RgbLedBlock(0x02)
  # ___planner.repeat(1, rgb.toggle)

  button = ButtonBlock(measurement_period=0.01)
  button.is_pressed.changed(True, button_pressed, button, rgb)

  logging.info("done")
plan()





