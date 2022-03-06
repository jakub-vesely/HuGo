#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from blocks.display_block import DisplayBlock
from basal.logging import Logging

def plan():
  logging = Logging()

  display = DisplayBlock()
  dim_x, dim_y = display.get_dimensions()

  logging.info("resolution x: %d, y: %d" % (dim_x, dim_y))
  display.draw_ellipse(int(dim_x / 2), int(dim_y / 2), int(dim_x /2) - 5, int(dim_y / 2) - 5)

  text = "Hallo"
  letter_width = 8
  letter_height = 8

  text_x = int(dim_x / 2) - int(len(text) * letter_width / 2) + 1
  text_y = int(dim_y / 2) - int(letter_height / 2) - 1

  display.print_text(text_x, text_y, text)
  line_y = text_y + letter_height + 2
  display.draw_line(text_x, line_y, text_x + len(text) * letter_width - 2, line_y)

  display.showtime()

plan()
