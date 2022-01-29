#include "motor.h"
#include <arduino.h>

Motor::Motor(uint8_t pinMA, uint8_t pinMB, uint8_t pinSensor):
  m_counter(0),
  m_clockwise(true),
  m_speed(0),
  m_pinMA(pinMA),
  m_pinMB(pinMB),
  m_pinSensor(pinSensor){

    pinMode(pinMA, OUTPUT);
    digitalWrite(pinMA, LOW);

    pinMode(pinMB, OUTPUT);
    digitalWrite(pinMB, LOW);

    pinMode(pinSensor, INPUT_PULLUP);
}

void Motor::processCommand(uint8_t command, uint8_t extra_byte, wire_buffer_t &write_buffer){    
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
      
    case I2C_MOTOR_RESET_COUNTER:
      noInterrupts();
      m_counter = 0;
      interrupts();
      break;
      
    case I2C_MOTOR_GET_COUNTER: {
      noInterrupts();
      uint32_t counter = m_counter;
      interrupts();
      
      uint8_t data[4];
      for (uint8_t index = 0; index < 4; ++index){
        write_buffer.data[3-index] = (counter >> (index * 8)) & 0xff; //big endian
      }
      write_buffer.size = 4;
    }
      break;
      
    case I2C_MOTOR_SET_SPEED:
      m_speed = extra_byte; // expected number from 0 to 100
      break;
  }
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
  else if (loopCounter == m_speed){
    digitalWrite(m_pinMA, LOW);
    digitalWrite(m_pinMB, LOW);
  }
}
