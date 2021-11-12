import machine
import ___planner
from ___ble import Ble

ble = Ble()

def _reboot():
  machine.reset()

def reboot():
  print("rebooting")
  ble.disconnect()
  ___planner.postpone(0.1, _reboot)

def run():
  ble.get_shell().load_events()
  ___planner.run()
  print("program terminated")
