#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

import math
from basal.logging import Logging
from blocks.motion_tracking_block import MotionTrackingBlock
from blocks.display_block import DisplayBlock
from blocks.button_block import ButtonBlock
from basal.planner import Planner

class Plan():
  counter = 0
  shown_id = 0
  threshold = 0.1
  gyro_angle = [0, 0, 0]
  gyro_period = 0.05
  logging = Logging("events")
  def __init__(self) -> None:
    Planner.postpone(2, self.init)

  def init(self):
    self.block = MotionTrackingBlock()
    self.display = DisplayBlock()
    self.display.clean()
    self.display.showtime()

    self.button = ButtonBlock()
    self.button.value.changed(self.buttonChanged)

    self.block.acceleration.set_change_threshold(0.1)
    # self.block.acceleration.changed(self.acceleration_changed)

    if not self.block.is_gyro_calibrated():
      self.logging.info("gyro calibration...")
      Planner.plan(self.gyro_calibration)

    self.block.gyro.set_change_threshold(0.1)
    self.block.gyro.change_period(self.gyro_period)

    if not self.block.is_magnetic_calibrated():
      self.logging.info("magnetic calibration...")
      Planner.plan(self.mag_calibration)
    self.block.magnetic.set_change_threshold(0.1)

    self.block.temperature.set_change_threshold(0.1)

    self.change_mode()

  def change_mode(self):
    self.block.acceleration.remove_all_triggers()
    self.block.gyro.remove_all_triggers()
    self.block.magnetic.remove_all_triggers()
    self.block.temperature.remove_all_triggers()

    if self.shown_id == 0:
      self.block.acceleration.changed(self.acceleration_changed)
    elif self.shown_id == 1:
      self.block.gyro.updated(self.gyro_updated)
    elif self.shown_id == 2:
      self.block.magnetic.changed(self.magnetic_changed)
    elif self.shown_id == 3:
      self.block.temperature.changed(self.temperature_changed)

  def buttonChanged(self):
    if self.button.value.get():
      self.shown_id += 1
      if self.shown_id == 4:
        self.shown_id = 0

      self.change_mode()

  def mag_calibration(self):
    self.block.calibrate_magnetic()
    self.logging.info("mag calibration DONE")

  def gyro_calibration(self):
    self.block.calibrate_gyro()
    self.logging.info("gyro calibration DONE")

  def coord_changed(self, coordinates, name):
    #self.logging.info(name + "coord:", coordinates)

    self.show_orientation(coordinates, name)
    #self.logging.info(name + " angle:", self.get_angle(coordinates))

  def acceleration_changed(self):
    coord = self.block.acceleration.get()
    if coord:
      self.coord_changed((-coord[1]*3, -coord[0]*3, coord[2]*3), "accel")

  def gyro_updated(self):
    coords = self.block.gyro.get()
    self.gyro_angle[0] += coords[0] * self.gyro_period
    self.gyro_angle[1] += coords[1] * self.gyro_period
    self.gyro_angle[2] += coords[2] * self.gyro_period

    self.display.clean()
    self.display.print_text(1, 1, str(self.gyro_angle[0]))
    self.display.print_text(1, 11, str(self.gyro_angle[1]))
    self.display.print_text(1, 21, str(self.gyro_angle[2]))
    self.display.showtime()

    #self.logging.info("gyro", self.gyro_angle, coords)

  def magnetic_changed(self):
    self.coord_changed(self.block.magnetic.get(), "mag")

  def temperature_changed(self):
    temp = self.block.temperature.get_str_to_fit(8)
    self.logging.info("temperature:", temp)
    self.display.clean()
    self.display.print_text(1, 1, "temp")
    self.display.print_text(1, 10, temp)
    self.display.showtime()

  def get_angle(self, coordinates):
    if (coordinates[1] == 0):
      if coordinates[0] > 0:
        return 90
      else:
        return -90

    return math.atan(coordinates[0]/coordinates[1]) * 180 / math.pi

  def show_orientation(self, coordinates, name):
    x = coordinates[0]
    y = coordinates[1]
    z = coordinates[2]

    dimensions = self.display.get_dimensions()
    center_x = round(dimensions[0] / 2)
    center_y = round(dimensions[1] / 2)

    self.display.clean()
    self.display.print_text(1,1,name)
    self.display.draw_line(center_x, center_y, round(center_x + x), round(center_y + y))
    self.display.draw_ellipse(center_x, center_y, round(abs(z/2)), round(abs(z/2)))
    self.display.showtime()

Plan()
