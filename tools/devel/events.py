import ___planner as planner
from ___logging import Logging
from ___rgb_led_block import RgbLedBlock
from ___display_block import DisplayBlock
from ___button_block import ButtonBlock
import random
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

# def current_changed(power_block):
#   logging.info("current changed: %.2f", power_block.battery_current_mA.get_value())

# def sensor_changed_1(driver):
#   logging.info("m1: %d", driver.motor1_sensor_counter.get_value())

# def sensor_changed_2(driver):
#   logging.info("m2: %d", driver.motor2_sensor_counter.get_value())

#toggle = False
#chassis = Chassis(None, 0x11, 0x12)


# def button_pressed():
#   global toggle
#   #logging.info("button: %d", button.is_pressed.get_value())
#   rgb.set_on() if button.is_pressed.get_value() else rgb.set_off()

#   if button.is_pressed.get_value():
#     toggle = not toggle

#     if toggle:
#       chassis.go_forward(100)
#       chassis.turn_right(10)
#     else:
#       chassis.stop()

class Circle():
  def __init__(self):
    self.x = int(random.random() * 20)
    self.y = int(random.random() * 20)
    self.r = 1

class Plan:
  def __init__(self):
    self.circles = list()

    # testing virtual keyboard
    #$___main_block.ble.get_keyboard().add_callback("q", pressed_q, logging, message="ahoj")
    self.intro_in_progress = True
    self.rgb = RgbLedBlock()
    self.display = DisplayBlock()
    self.button = ButtonBlock()

    if self.button.is_available() and self.rgb.is_available():
      self.button.is_pressed.changed(True, self.show_button_pressed)

    if self.display.is_available():
      dim_x, dim_y = self.display.get_dimensions()
      self.center_x = int(dim_x / 2)
      self.center_y = int(dim_y / 2)

      self.circles.append(Circle())
      if self.button.is_available():
        self.button.is_pressed.more_than(0, False, self.disply_button_pressed)

    #___main_block.ble.get_keyboard().add_callback("r", pressed_r, rgb)

    # rgb = RgbLedBlock(0x02)
    # ___planner.repeat(1, rgb.toggle)

    #button = ButtonBlock(measurement_period=0.01)
    #button.is_pressed.changed(True, button_pressed, button, rgb)
    logging.info("plan initialized")

  def show_button_pressed(self):
    self.rgb.set_on() if self.button.is_pressed.get_value() else self.rgb.set_off()

  def disply_button_pressed(self):
    state  = "On" if self.button.is_pressed.get_value() else "Off"
    self.display.clean()
    self.display.print_text(0,0, "Button")
    self.display.print_text(0,8, state)
    self.display.showtime()



Plan()





