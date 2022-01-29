from planner import Planner
from logging import Logging
import machine
import time
from micropython import const

import gc
start_free = gc.mem_free()
from button_block import ButtonBlock
print (start_free - gc.mem_free())

import_free = gc.mem_free()
buttonBlock = ButtonBlock()
print (import_free - gc.mem_free())


import_free = gc.mem_free()
buttonBlock = ButtonBlock(0x01)
print (import_free - gc.mem_free())
