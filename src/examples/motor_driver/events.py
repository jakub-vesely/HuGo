#  Copyright (c) 2022 Jakub Vesely
#  This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

from basal.ble import Ble
from basal.logging import Logging
from basal.planner import Planner
from blocks.motor_driver_block import MotorDriverBlock
from remote_control.remote_key import RemoteKey

class Plan():
  fullspeed = 100
  logging = Logging("events")

  def __init__(self) -> None:
    self.speed = 0
    self.pwm = 50

    Ble.value_remote.equal_to(RemoteKey("a"), True, self.pwm_down)
    Ble.value_remote.equal_to(RemoteKey("d"), True, self.pwm_up)
    Ble.value_remote.equal_to(RemoteKey("w"), True, self.speed_up)
    Ble.value_remote.equal_to(RemoteKey("s"), True, self.slow_down)
    Ble.value_remote.equal_to(RemoteKey("z"), True, self.stop)
    Ble.value_remote.equal_to(RemoteKey("o"), True, self.full_speed_up)
    Ble.value_remote.equal_to(RemoteKey("l"), True, self.full_speed_down)

    self.motor_driver_front = MotorDriverBlock(0x20)
    #self.motor_driver_front.change_block_address(0x20)
    self.motor_driver_front.turn_clockwise(MotorDriverBlock.motor1_id)
    self.motor_driver_front.turn_opposite(MotorDriverBlock.motor2_id)

    self.motor_driver_rear = MotorDriverBlock(0x21)
    #self.motor_driver_rear.change_block_address(0x21)
    self.motor_driver_rear.turn_clockwise(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.turn_opposite(MotorDriverBlock.motor2_id)

    self.motor_driver_front.sensor_power_on()
    self.motor_driver_rear.sensor_power_on()
    self.motor_driver_front.reset_sensor_counter(MotorDriverBlock.motor1_id)
    self.motor_driver_front.reset_sensor_counter(MotorDriverBlock.motor2_id)
    self.motor_driver_rear.reset_sensor_counter(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.reset_sensor_counter(MotorDriverBlock.motor2_id)

    Planner.repeat(1, self.print_counters)

  def print_counters(self):
    counter11 = self.motor_driver_front.get_sensor_counter(MotorDriverBlock.motor1_id)
    counter12 = self.motor_driver_front.get_sensor_counter(MotorDriverBlock.motor2_id)
    counter21 = self.motor_driver_rear.get_sensor_counter(MotorDriverBlock.motor1_id)
    counter22 = self.motor_driver_rear.get_sensor_counter(MotorDriverBlock.motor2_id)
    self.logging.info("counter11: {0}, counter12: {1}, counter21: {2}, counter22: {3}".format(counter11, counter12, counter21, counter22))

  def _adjust_motors(self):
    self.logging.info("fullspeed: %d, speed: %d, pwm: %d" % (self.fullspeed, self.speed, self.pwm))
    fullspeed = 0
    if self.fullspeed > 0:
      self.motor_driver_front.turn_clockwise(MotorDriverBlock.motor1_id)
      self.motor_driver_rear.turn_clockwise(MotorDriverBlock.motor1_id)
      fullspeed = self.fullspeed
    else:
      self.motor_driver_front.turn_opposite(MotorDriverBlock.motor1_id)
      self.motor_driver_rear.turn_opposite(MotorDriverBlock.motor1_id)
      fullspeed = self.fullspeed * -1

    self.motor_driver_front.set_speed(MotorDriverBlock.motor1_id, fullspeed)
    self.motor_driver_front.set_speed(MotorDriverBlock.motor2_id, self.speed)

    self.motor_driver_rear.set_speed(MotorDriverBlock.motor1_id, fullspeed)
    self.motor_driver_rear.set_speed(MotorDriverBlock.motor2_id, self.speed)
    self.motor_driver_front.set_pwm_period(self.pwm)
    self.motor_driver_rear.set_pwm_period(self.pwm)

  def full_speed_up(self):
    self.fullspeed += 10
    self._adjust_motors()

  def full_speed_down(self):
    self.fullspeed -=10
    self._adjust_motors()

  def pwm_up(self):
    self.pwm += 10
    self._adjust_motors()

  def pwm_down(self):
    self.pwm -= 10
    self._adjust_motors()

  def speed_up(self):
    self.speed += 5
    self._adjust_motors()

  def slow_down(self):
    self.speed -= 5
    self._adjust_motors()

  def stop(self):
    self.motor_driver_front.stop(MotorDriverBlock.motor1_id)
    self.motor_driver_front.stop(MotorDriverBlock.motor2_id)
    self.motor_driver_rear.stop(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.stop(MotorDriverBlock.motor2_id)
Plan()