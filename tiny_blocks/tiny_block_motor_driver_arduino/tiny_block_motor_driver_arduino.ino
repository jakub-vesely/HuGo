/*
power board management

BOARD: base+power
Chip: ATtiny414
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <Wire.h>
#include <avr/sleep.h>

#include <hugo_tiny_wire.h>
#include <hugo_defines.h>
#include "motor.h"

#define INA1 PIN_PA5
#define INB1 PIN_PB2
#define LED1 PIN_PA7
#define FOTO1 PIN_PA6

#define INA2 PIN_PA3
#define INB2 PIN_PA4
#define LED2 PIN_PA1
#define FOTO2 PIN_PA2

static Motor motor1(INA1, INB1, FOTO1, LED1);
static Motor motor2(INA2, INB2, FOTO2, LED2);

static uint8_t loop_counter = 0;

void sensor1Interrupt(){
  motor1.incrementCounter();
}

void sensor2Interrupt(){
  motor2.incrementCounter();
}

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  if (payload_size > 0){
    uint8_t motor_id = Wire.read();
    uint8_t extra_byte = 0;
    if (payload_size == 2){
      extra_byte = Wire.read();
    }
    if (motor_id == 1){
      motor1.processCommand(command, extra_byte, s_buffer);
    }
    else if (motor_id == 2){
      motor2.processCommand(command, extra_byte, s_buffer);
    }
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_MOTOR_DRVR;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void setup(){
  attachInterrupt(digitalPinToInterrupt(FOTO1), sensor1Interrupt, CHANGE); //must be static fuction (can't be part of the motor class)
  attachInterrupt(digitalPinToInterrupt(FOTO2), sensor2Interrupt, CHANGE);

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_MOTOR_DRVR, NULL);
}

void loop(){

  motor1.processLoopIteration(loop_counter);
  motor2.processLoopIteration(loop_counter);

  loop_counter++;
  if (loop_counter > 9){
    loop_counter = 0;
  }

  delayMicroseconds(500);
}
