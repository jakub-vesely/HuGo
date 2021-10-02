/*
power board management

BOARD: base+power
Chip: ATtiny414
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <Wire.h>
#include <avr/sleep.h>
#include "motor_commands.h"
#include "motor.h"
#include <hugo_tiny_block_base_arduino.h>
#include <hugo_tiny_block_defines.h>

#define INA1 PIN_PA5
#define INB1 PIN_PB2
#define LED1 PIN_PA7
#define FOTO1 PIN_PA6

#define INA2 PIN_PA3
#define INB2 PIN_PA4
#define LED2 PIN_PA1
#define FOTO2 PIN_PA2

static HugoTinyBlockBaseArduino block(I2C_BLOCK_TYPE_ID_MOTOR_DRVR);

Motor motor1(INA1, INB1, FOTO1, LED1);
Motor motor2(INA2, INB2, FOTO2, LED2);

uint8_t loop_counter = 0;
bool rotated = false;
void sensor1Interrupt(){
  motor1.incrementCounter();
}

void sensor2Interrupt(){
  motor2.incrementCounter();
}

void i2c_receive_data(int count) {
  uint8_t command = block.ReceiveI2cCommand(count);
  if (block.GetI2cPayloadSize(count) > 0){
    uint8_t motor = Wire.read();
    if (motor == 1){
      motor1.processCommand(command);
    }
    else if (motor == 2){
      motor2.processCommand(I2C_MOTOR_SET_SPEED_100);
      motor2.processCommand(command);
    }
  }
}

void i2c_request_event(){
  int32_t counter = 0;
  if (motor1.isDataReady()){
    counter = motor1.getCounter();
  }
  else if (motor2.isDataReady()){
    counter = motor2.getCounter();
  }
  //to do not be endian dependent
  uint8_t data[4];
  for (uint8_t index = 0; index < 4; ++index){
    data[index] = (counter >> (index * 8)) & 0xff;
  }

  Wire.write(data, 4);
}

void setup(){
  attachInterrupt(digitalPinToInterrupt(FOTO1), sensor1Interrupt, CHANGE); //must be static fuction (can't be part of the motor class)
  attachInterrupt(digitalPinToInterrupt(FOTO2), sensor2Interrupt, CHANGE);

  Wire.begin(block.GetI2cAddress());
  Wire.onReceive(i2c_receive_data);
  Wire.onRequest(i2c_request_event);

  //motor1.processCommand(I2C_MOTOR_SET_SPEED_100);
  //motor2.processCommand(I2C_MOTOR_SET_SPEED_100);
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
