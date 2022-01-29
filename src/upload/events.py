import planner
from logging import Logging

logging = Logging("events")

def print_message(message):
  logging.info(message)

def plan():
  # testing planner
  planner.postpone(0.01, print_message, "Hello World")

  # testing logging
  logging.info("events planned")

plan()





