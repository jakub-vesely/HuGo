/*
power board management

BOARD: base+power
Chip: ATtiny414
Clock Speed: 20MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <Wire.h>
#include <avr/sleep.h>
#include "motor_commands.h"
#include "motor.h"

#define INA1 PIN_PA5
#define INB1 PIN_PB2
#define LED1 PIN_PA7
#define FOTO1 PIN_PA6

#define INA2 PIN_PA3
#define INB2 PIN_PA4
#define LED2 PIN_PA1
#define FOTO2 PIN_PA2

Motor motor1(INA1, INB1, FOTO1, LED1);
Motor motor2(INA2, INB2, FOTO2, LED2);

uint8_t counter = 0;
bool rotated = false;
void sensor1Interrupt(){
  motor1.incrementCounter();
}

void sensor2Interrupt(){
  motor2.incrementCounter();
}

void i2c_receive_data(int count) {
  if (count == 2){
    uint8_t command = Wire.read();
    uint8_t motor = Wire.read();
    if (motor == 1){
      motor1.processCommand(command);
    }
    else{
      motor2.processCommand(command);
    }
  }
}

void i2c_request_event(){
  int32_t counter = motor1.isDataReady() ? motor1.getCounter() : motor2.getCounter();
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

  Wire.begin(0x03);
  Wire.onReceive(i2c_receive_data);
  Wire.onRequest(i2c_request_event);
}

void loop(){

  motor1.processLoopIteration(counter);
  motor2.processLoopIteration(counter);

  counter++;
  if (counter > 9){
    counter = 0;
  }

  delayMicroseconds(500);
}
