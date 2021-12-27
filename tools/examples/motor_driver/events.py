from motor_driver_block import MotorDriverBlock
from logging import Logging

logging = Logging("events")

def sensor_changed(driver):
  logging.info(driver.motor2_sensor_counter.get_value())

def plan():
  motor_driver = MotorDriverBlock(0x02)

  motor_driver.turn_opposite(motor_driver.motor1_id)
  motor_driver.set_speed(motor_driver.motor1_id, 10)

  motor_driver.sensor_power_on(motor_driver.motor2_id)
  motor_driver.motor2_sensor_counter.changed(True, sensor_changed, motor_driver)
  motor_driver.turn_opposite(motor_driver.motor2_id)
  motor_driver.set_speed(motor_driver.motor2_id, 10)


plan()