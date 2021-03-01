#include "motor.h"
#include "motor_commands.h"
#include <arduino.h>

Motor::Motor(uint8_t pinMA, uint8_t pinMB, uint8_t pinSensor, uint8_t pinPower):
  m_counter(0),
  m_clockwise(true),
  m_speed(0),
  m_pinMA(pinMA),
  m_pinMB(pinMB),
  m_pinSensor(pinSensor),
  m_pinPower(pinPower){

    pinMode(pinMA, OUTPUT);
    digitalWrite(pinMA, LOW);

    pinMode(pinMB, OUTPUT);
    digitalWrite(pinMB, LOW);

    pinMode(pinPower, OUTPUT);
    digitalWrite(pinPower, LOW);

    pinMode(pinSensor, INPUT);
}

void Motor::processCommand(uint8_t command){
  switch (command) {
    case I2C_MOTOR_TURN_CLOCKWISE:
      m_clockwise = true;
      break;
    case I2C_MOTOR_TURN_ANTICLOCKWISE:
      m_clockwise = false;
      break;
    case I2C_MOTOR_STOP:
      m_speed = 0;
      break;
    case I2C_MOTOR_POWER_OFF:
      m_speed = 0;
      digitalWrite(m_pinPower, LOW);
      break;
    case I2C_MOTOR_POWER_ON:
      digitalWrite(m_pinPower, HIGH);
      break;
    case I2C_MOTOR_RESET_COUNTER:
      noInterrupts();
      m_counter = 0;
      interrupts();
      break;
    case I2C_MOTOR_GET_COUNTER:
      m_dataReady = true;
      break;
    case I2C_MOTOR_SET_SPEED_100:
      m_speed = 100;
      break;
    case I2C_MOTOR_SET_SPEED_80:
      m_speed = 80;
      break;
    case I2C_MOTOR_SET_SPEED_60:
      m_speed = 60;
      break;
    case I2C_MOTOR_SET_SPEED_40:
      m_speed = 40;
      break;
    case I2C_MOTOR_SET_SPEED_20:
      m_speed = 20;
      break;
    case I2C_MOTOR_SET_SPEED_0:
      m_speed = 0;
      break;
  }
}

uint32_t Motor::getCounter(){
  noInterrupts();
  uint32_t counter = m_counter;
  interrupts();
  m_dataReady = false;
  return counter;
}

void Motor::processLoopIteration(uint8_t loopCounter){
  if (loopCounter == 0 && m_speed != 0) {
    if (m_clockwise){
      digitalWrite(m_pinMA, HIGH);
      digitalWrite(m_pinMB, LOW);
    }
    else{
      digitalWrite(m_pinMA, LOW);
      digitalWrite(m_pinMB, HIGH);
    }
  }
  else if (loopCounter * 10 == m_speed){
    digitalWrite(m_pinMA, LOW);
    digitalWrite(m_pinMB, LOW);
  }
}
