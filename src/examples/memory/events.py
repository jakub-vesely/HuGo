#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

import machine
import time
from micropython import const

from basal.planner import Planner
from basal.logging import Logging

import gc
start_free = gc.mem_free()
from blocks.button_block import ButtonBlock

print (start_free - gc.mem_free())

import_free = gc.mem_free()
buttonBlock = ButtonBlock()
print (import_free - gc.mem_free())


import_free = gc.mem_free()
buttonBlock = ButtonBlock(0x01)
print (import_free - gc.mem_free())
