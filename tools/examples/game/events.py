from logging import Logging
import logging
import planner as planner
from logging import Logging
from rgb_led_block import RgbLedBlock
from display_block import DisplayBlock
from button_block import ButtonBlock
import random

class Map:
  def __init__(self, dim_x, dim_y) -> None:
    self.logging = Logging("map")
    self.top = []
    self.bottom = []
    self.current_x = 0
    self.dim_x = dim_x
    self.dim_y = dim_y
    self.removed_count = 0

  def add_borders(self, top_y, bottom_y, x):
    for x0 in range(x):
      self.top.append(top_y)
      self.bottom.append(bottom_y)

  def get_borders(self, x):
    pos = self.current_x + x - self.removed_count
    try:
      return self.top[pos], self.dim_y - self.bottom[pos]
    except Exception:
      self.logging.error(pos)


  def add_random_layers(self, count):
    for _index in range(count):
      top_min = 9
      bottom_min = 0
      top_max = 18
      bottom_max = 18
      last_top = self.top[-1]
      last_bottom = self.bottom[-1]
      change_top = int(random.random() * 3) - 1
      change_bottom = int(random.random() * 3) - 1
      top_pos =  last_top - change_top if last_top - change_top > top_min else top_min
      bottom_pos =  last_bottom - change_bottom if last_bottom - change_bottom > bottom_min else bottom_min
      if top_pos + bottom_pos > self.dim_y - 20:
        if top_pos - top_min > bottom_pos:
          top_pos -= top_pos + bottom_pos - (self.dim_y - 20)
        else:
          bottom_pos -= top_pos + bottom_pos - (self.dim_y - 20)
      self.add_borders(top_pos, bottom_pos, 1)

  def remove_first_layers(self, count):
    self.top = self.top[count:]
    self.bottom = self.bottom[count:]
    self.removed_count += count

  def start(self):
    self.current_x = 0

  def is_enough_data(self):
    return self.current_x - self.removed_count + self.dim_x < len(self.top)

  def draw(self, display:DisplayBlock):
    for x in range(self.dim_x):
      pos = self.current_x + x - self.removed_count
      display.draw_line(x, self.top[pos], x, 0)
      display.draw_line(x, self.dim_y - self.bottom[pos] - 1, x, self.dim_y - 1)

class Point:
  def __init__(self, y0) -> None:
    self.y = y0
    self.x = 5
    self.r = 3

  def draw(self, display:DisplayBlock):
    for r in range(self.r):
      display.draw_ellipse(self.x, self.y, r, r)
class Plan:
  def __init__(self):
    self.started = False
    self.display = DisplayBlock()
    self.logging = Logging("plan")
    self.button = ButtonBlock()
    self.dim_x, self.dim_y = self.display.get_dimensions()
    self.center_x = int(self.dim_x / 2)
    self.center_y = int(self.dim_y / 2)

    self.point = Point(self.center_y)
    self.map = Map(self.dim_x, self.dim_y)
    self.init_map(self.map)
    #TODO: not repetitive event
    #self.button.is_pressed.changed(False, self.start_button)
    self.redraw()

  def start_button(self):
    self.started = True
    self.logging.info("started")
    self.redraw()

  def init_map(self, map:Map):
    self.map.add_borders(11, 2, 10)
    self.map.add_random_layers(100)

        #break
    map.start()


  def redraw(self):
    if not self.started and self.button.is_pressed.get_value():
      self.started = True

    speed = int(self.map.current_x / 200) + 1
    if speed > 30:
      speed = 30

    if not self.map.is_enough_data():
      remove_step_count = speed
      self.map.remove_first_layers(remove_step_count)
      self.map.add_random_layers(remove_step_count)
      #self.logging.info((self.map.current_x, self.map.removed_count, len(self.map.top)))

    self.display.clean()
    self.point.draw(self.display)
    if self.started:
      if self.button.is_pressed.get_value():
        self.point.y -= 2
      else:
        self.point.y += 2

    border_top, border_bottom = self.map.get_borders(self.point.x)

    touch = False
    if self.point.y < border_top + self.point.r - 1:
      self.point.y = border_top + self.point.r
      touch = True

    if self.point.y > border_bottom - self.point.r:
      self.point.y = border_bottom - self.point.r
      touch = True

    self.display.invert(False if touch else True)

    self.map.draw(self.display)
    if self.started:
      self.map.current_x += speed
    score = int(self.map.current_x / 10)
    self.display.print_text(0, 0, str(score), color=0)
    self.display.showtime()
    if not touch:
      planner.postpone(0.05, self.redraw)

Plan()