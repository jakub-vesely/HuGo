import machine
import gc
import ___planner
from ___ble import Ble

ble = Ble()

def _reboot():
  machine.reset()

def reboot():
  print("rebooting")
  ble.disconnect()
  ___planner.postpone(0.1, _reboot)

def get_mem_info():
    free = gc.mem_free()
    allocated = gc.mem_alloc()
    total = free + allocated
    percent = "{0:.2f} %".format(free /  total * 100)
    return "Free mem: {0} ({1})".format(free, percent)

def run():
  ble.get_shell().load_events()
  ___planner.run()
  print("program terminated")
