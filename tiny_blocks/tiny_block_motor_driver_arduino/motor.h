#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <hugo_defines.h>

class Motor {
private:
  sensorInterrupt();

  volatile uint32_t m_counter; //motor rotation sensor changes
  bool m_clockwise; //rotation direction
  uint8_t m_speed; //expected from 0 to 10

  uint8_t m_pinMA; //motor pin A number
  uint8_t m_pinMB; //motor pin B number
  uint8_t m_pinSensor; //rotation sensor pin number
  uint8_t m_pinPower; //led pin number

public:
  Motor(uint8_t pinMA, uint8_t pinMB, uint8_t pinSensor, uint8_t pinPower);

  void processCommand(uint8_t command, uint8_t payload_size, wire_buffer_t &write_buffer);
  inline void incrementCounter() { m_counter++; }
  void processLoopIteration(uint8_t loopCounter); //loopCounter is expected in range 0-10
};
