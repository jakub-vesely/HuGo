from planner import Planner

def print_message(mesage):
  print(mesage)

def plan():
  planner = Planner()
  planner.postpone(0.01, print_message, "Hello World")

plan()