from motor_driver_block import MotorDriverBlock, Speed
from logging import Logging
import main_block
import planner
from power_block import PowerBlock

logging = Logging("events")

class Plan():
  fullspeed =  100
  def __init__(self) -> None:
    self.speed = 0
    self.pwm = 50

    self.logging = Logging("plan")
    main_block.ble.get_keyboard().add_callback("a", self.pressed_a)
    main_block.ble.get_keyboard().add_callback("d", self.pressed_d)
    main_block.ble.get_keyboard().add_callback("w", self.pressed_w)
    main_block.ble.get_keyboard().add_callback("s", self.pressed_s)
    main_block.ble.get_keyboard().add_callback("z", self.pressed_z)
    main_block.ble.get_keyboard().add_callback("o", self.pressed_o)
    main_block.ble.get_keyboard().add_callback("l", self.pressed_l)

    self.motor_driver_front = MotorDriverBlock(0x11)
    self.motor_driver_front.turn_clockwise(MotorDriverBlock.motor1_id)
    self.motor_driver_front.turn_opposite(MotorDriverBlock.motor2_id)

    self.motor_driver_rear = MotorDriverBlock(0x12)
    self.motor_driver_rear.turn_clockwise(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.turn_opposite(MotorDriverBlock.motor2_id)

    self.motor_driver_front.sensor_power_on()
    self.motor_driver_rear.sensor_power_on()
    self.motor_driver_front.reset_sensor_counter(MotorDriverBlock.motor1_id)
    self.motor_driver_front.reset_sensor_counter(MotorDriverBlock.motor2_id)
    self.motor_driver_rear.reset_sensor_counter(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.reset_sensor_counter(MotorDriverBlock.motor2_id)

    planner.repeat(1, self.print_counters)

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

  def pressed_o(self):
    self.fullspeed += 10
    self._adjust_motors()

  def pressed_l(self):
    self.fullspeed -=10
    self._adjust_motors()

  def pressed_d(self):
    self.pwm += 10
    self._adjust_motors()

  def pressed_a(self):
    self.pwm -= 10
    self._adjust_motors()

  def pressed_w(self):
    self.speed += 5
    self._adjust_motors()

  def pressed_s(self):
    self.speed -= 5
    self._adjust_motors()

  def pressed_z(self):
    self.motor_driver_front.stop(MotorDriverBlock.motor1_id)
    self.motor_driver_front.stop(MotorDriverBlock.motor2_id)
    self.motor_driver_rear.stop(MotorDriverBlock.motor1_id)
    self.motor_driver_rear.stop(MotorDriverBlock.motor2_id)
Plan()