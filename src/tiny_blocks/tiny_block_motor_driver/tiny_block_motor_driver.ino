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

#define PCB_VERSION 2

#define INA1 PIN_PA5
#define INB1 PIN_PB2
#define FOTO1 PIN_PA6

#define INA2 PIN_PA3
#define INB2 PIN_PA4
#define FOTO2 PIN_PA2

# define SENSOR_LED PIN_PB3

static Motor motor1(INA1, INB1, FOTO1);
static Motor motor2(INA2, INB2, FOTO2);

static uint8_t s_loop_counter = 0;
static uint8_t s_pwm_period = 10; //10 is equal to 100us

void sensor1Interrupt(){
  motor1.incrementCounter();
}

void sensor2Interrupt(){
  motor2.incrementCounter();
}

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
    switch (command){
    case I2C_MOTOR_POWER_ON:
      digitalWrite(SENSOR_LED, HIGH);
      break;
    case I2C_MOTOR_POWER_OFF:
      digitalWrite(SENSOR_LED, LOW);
      break;
    case I2C_MOTOR_SET_PWM_PERIOD:
      if (payload_size == 1){
        s_pwm_period = Wire.read();
      }
      break;
    default:
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
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_MOTOR_DRIVER;
  s_buffer.data[1] = PCB_VERSION;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup(){
  attachInterrupt(digitalPinToInterrupt(FOTO1), sensor1Interrupt, CHANGE); //must be static fuction (can't be part of the motor class)
  attachInterrupt(digitalPinToInterrupt(FOTO2), sensor2Interrupt, CHANGE);

  pinMode(SENSOR_LED, OUTPUT);
  digitalWrite(SENSOR_LED, LOW);

  (I2C_BLOCK_TYPE_ID_MOTOR_DRIVER, NULL);
  motor1.initialize();
  motor2.initialize();
}

void loop(){
  // motor1.processLoopIteration(s_loop_counter);
  // motor2.processLoopIteration(s_loop_counter);

  // s_loop_counter++;
  // if (s_loop_counter > 99){
  //   s_loop_counter = 0;
  // }

  // delayMicroseconds((uint16_t)s_pwm_period * 10); //one pwm_period step is 10 us
  motor1.AtoB();
  motor2.ABoff();
  delay(2);
  motor2.BtoA();
  motor1.ABoff();
  delay(2);
  motor1.BtoA();
  motor2.ABoff();
  delay(2);
  motor2.AtoB();
  motor1.ABoff();
  delay(2);
}
