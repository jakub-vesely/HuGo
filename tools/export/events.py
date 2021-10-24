import planner
from logging import Logging
import main_block

def print_message(mesage):
  print(mesage)

def pressed_q(logging , message):
  logging.info(message)
  #main_block.ble.get_keyboard().del_callback("q", pressed_q)

def plan():
  # testing planner
  planner.postpone(0.01, print_message, "Hello World")

  # testing logging
  logging = Logging("main")
  logging.warning("testing log")

  # testing virtual keyboard
  main_block.ble.get_keyboard().add_callback("q", pressed_q, logging, message="ahoj")

plan()



