#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

import random
import gc
from basal.logging import Logging
from basal.planner import Planner
from blocks.rgb_led_block import RgbLedBlock, RgbLedBlockColor
from blocks.display_block import DisplayBlock
from blocks.button_block import ButtonBlock
from blocks.sound_block import SoundBlock

import machine

class Diamond:
  base_dim = 8 #dimension
  dim_diff = 2
  max_dim = base_dim + dim_diff

  def __init__(self, y, is_top, dimension) -> None:
     self.y = y
     self.is_top = is_top
     self.dim = dimension
     self.half_dim = int(dimension / 2)

  def _draw_top(self, x, display:DisplayBlock):
    display.draw_line(x - self.dim, self.y, x + self.dim, self.y)
    display.draw_line(x - self.dim, self.y, x, self.y - self.dim)
    display.draw_line(x, self.y - self.dim, x + self.dim, self.y)
    display.draw_line(x, self.y, x, self.y - self.dim)

    display.draw_line(x - self.dim, self.y, x - self.half_dim, self.y + self.half_dim)
    display.draw_line(x + self.dim, self.y, x + self.half_dim, self.y + self.half_dim)
    display.draw_line(x - self.half_dim, self.y + self.half_dim, x + self.half_dim, self.y + self.half_dim)
    display.draw_line(x - self.half_dim, self.y + self.half_dim, x, self.y)
    display.draw_line(x + self.half_dim, self.y + self.half_dim, x, self.y)

  def _draw_bottom(self, x, display:DisplayBlock):
    display.draw_line(x - self.dim, self.y, x + self.dim, self.y)
    display.draw_line(x - self.dim, self.y, x, self.y + self.dim)
    display.draw_line(x, self.y + self.dim, x + self.dim, self.y)
    display.draw_line(x, self.y, x, self.y + self.dim)

    display.draw_line(x - self.dim, self.y, x - self.half_dim, self.y - self.half_dim)
    display.draw_line(x + self.dim, self.y, x + self.half_dim, self.y - self.half_dim)
    display.draw_line(x - self.half_dim, self.y - self.half_dim, x + self.half_dim, self.y - self.half_dim)
    display.draw_line(x - self.half_dim, self.y - self.half_dim, x, self.y)
    display.draw_line(x + self.half_dim, self.y - self.half_dim, x, self.y)

  def draw(self, x, display:DisplayBlock):
    if self.is_top:
      self._draw_top(x, display)
    else:
      self._draw_bottom(x, display)

  def is_in_area(self, object_min, object_max):
    if self.is_top:
      return object_max > self.y - self.half_dim and object_min < self.y + self.dim
    else:
      return object_max > self.y - self.dim and object_min < self.y + self.half_dim

class MapLayer:
  def __init__(self, top, bottom, diamond):
    self.top = top
    self.bottom = bottom
    self.diamond = diamond

class Map:
  def __init__(self, dim_x, dim_y) -> None:
    self.logging = Logging("map")
    self.map_layers = []
    self.current_x = 0
    self.dim_x = dim_x
    self.dim_y = dim_y
    self.removed_count = 0
    self.init()

  def add_map_layer(self, top_y, bottom_y, diamond):
    self.map_layers.append(MapLayer(top_y, bottom_y, diamond))

  def get_borders(self, x):
    pos = self.current_x + x - self.removed_count
    try:
      layer = self.map_layers[pos]
      return layer.top, self.dim_y - layer.bottom
    except Exception:
      self.logging.error(pos)

  def remove_diamond(self, diamond):
    for layer in self.map_layers:
      if layer.diamond == diamond:
        layer. diamond = None

  def get_diamonds(self, x):
    diamonds = list()
    for current_x in range(x - int(Diamond.max_dim), x + int(Diamond.max_dim) + 1):

      pos = self.current_x + current_x - self.removed_count
      layer = self.map_layers[pos]
      if layer.diamond:
        diamonds.append(layer.diamond)
    return diamonds

  def add_random_layers(self, count):
    for _index in range(count):
      top_min = 8
      bottom_min = 0
      map_height = self.dim_y - 20 #20 is the border line height
      last_top = self.map_layers[-1].top
      last_bottom = self.map_layers[-1].bottom
      change_top = int(random.random() * 3) - 1
      change_bottom = int(random.random() * 3) - 1
      top_pos =  last_top - change_top if last_top - change_top > top_min else top_min
      bottom_pos =  last_bottom - change_bottom if last_bottom - change_bottom > bottom_min else bottom_min
      if top_pos + bottom_pos > map_height:
        if top_pos - top_min > bottom_pos:
          top_pos -= top_pos + bottom_pos - map_height
        else:
          bottom_pos -= top_pos + bottom_pos - map_height

      diamond = None
      if self.current_x + _index > 10 and (self.current_x + _index) % (Diamond.max_dim * 2) == 0 and int(random.random() * 10) > 5:
        is_top = True if int(random.random() * 2) else False
        dimension = int(random.random() * (Diamond.dim_diff * 2 + 1)) - Diamond.dim_diff + Diamond.base_dim

        embedding = 1 if dimension < Diamond.base_dim else 2
        if is_top:
          diamond = Diamond(top_pos + dimension - embedding, True, dimension)
        else:
          diamond = Diamond(self.dim_y - bottom_pos - dimension + embedding, False, dimension)
      self.add_map_layer(top_pos, bottom_pos, diamond)

  def remove_first_layers(self, count):
    self.map_layers = self.map_layers[count:]
    self.removed_count += count

  def init(self):
    self.map_layers = []
    self.add_map_layer(11, 2, None)
    self.add_random_layers(100)
    self.current_x = 0

  def is_enough_data(self):
    return self.current_x - self.removed_count + self.dim_x < len(self.map_layers)

  def draw(self, display:DisplayBlock):
    for x in range(self.dim_x):
      pos = self.current_x + x - self.removed_count
      if x >= 0:
        layer = self.map_layers[pos]
        display.draw_line(x, layer.top, x, 0)
        display.draw_line(x, self.dim_y - layer.bottom - 1, x, self.dim_y - 1)
        if layer.diamond:
          layer.diamond.draw(x, display)

class Point:
  def __init__(self, y0) -> None:
    self.y = y0
    self.x = 5
    self.r = 3

  def draw(self, display:DisplayBlock):
    for r in range(self.r):
      display.draw_ellipse(self.x, self.y, r, r)

class Plan:
  sound_folder = "miner"
  sound_start = "start"
  sound_pick = "pick"
  sound_crash = "crash"

  def __init__(self):
    self.started = False
    self.display = DisplayBlock()
    self.logging = Logging("plan")
    self.button = ButtonBlock()
    self.rgb = RgbLedBlock()
    self.sound = SoundBlock()
    self.sound.set_volume(5)
    #self.sound.play_by_folder_name(self.sound_folder, self.sound_start)
    self.led_default()
    self.dim_x, self.dim_y = self.display.get_dimensions()
    self.center_x = int(self.dim_x / 2)
    self.center_y = int(self.dim_y / 2)
    self.point = None
    self.map = None
    self.score = 0

    self.wait_for_start()

  def wait_for_start(self):
    #self.logging.info("wait_for_start")
    self.started = False
    self.score = 0
    self.point = Point(self.center_y)
    self.map = Map(self.dim_x, self.dim_y)
    self.display.invert(True)
    self.sound.play_by_folder_name(self.sound_folder, self.sound_start)
    self.button.value.equal_to_once(True, self.start_button)
    gc.collect() #force garbage collection before game starts
    self.logging.info("mem %s", str(gc.mem_free()))
    self.redraw()

  def start_button(self):
    self.started = True
    #self.logging.info("started")
    self.led_default()
    Planner.plan(self.redraw)

  def redraw(self):
    speed = int(self.map.current_x / 400) + 2
    if speed > 10:
      speed = 10

    if not self.map.is_enough_data():
      remove_step_count = speed
      self.map.remove_first_layers(remove_step_count)
      self.map.add_random_layers(remove_step_count)

    self.display.clean()
    self.point.draw(self.display)
    if self.started:
      if self.button.value.get(True): #no listener is active
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

    if not touch:
      diamonds = self.map.get_diamonds(self.point.x)
      for diamond in diamonds:
        if diamond.is_in_area(self.point.y - self.point.r, self.point.y + self.point.r):
          self.map.remove_diamond(diamond)
          self.score += 1
          self._pick_effect()
          Planner.postpone(0.05, self.led_default)

    self.map.draw(self.display)
    if self.started:
      self.display.invert(False if touch else True)
      self.map.current_x += speed
    #score = int(self.map.current_x / 10)
    self.display.print_text(0, 0, str(self.score), color=0)
    self.display.showtime()
    if self.started:
      if  not touch:
        Planner.postpone(0.05, self.redraw)
      else:
        self.started = False
        self._crash_effect()
        Planner.postpone(0.5, self.wait_for_reset_pressed) #wait a while to prevent random press and wait for starting new game

  def _pick_effect(self):
    self.rgb.set_color(RgbLedBlockColor.green)
    self.sound.play_by_folder_name(self.sound_folder, self.sound_pick)

  def _crash_effect(self):
    self.rgb.set_color(RgbLedBlockColor.red)
    self.sound.play_by_folder_name(self.sound_folder, self.sound_crash)
    #self.logging.info("crash")

  def led_default(self):
    self.rgb.set_color(RgbLedBlockColor.aquamarine)

  def wait_for_reset_pressed(self):
    #self.logging.info("wait_for_reset_pressed")
    self.button.value.equal_to_once(True, self.wait_for_reset_released)

  def wait_for_reset_released(self):
    #self.logging.info("wait_for_reset_released")
    self.button.value.equal_to_once(False, self.wait_for_start)

Plan()
