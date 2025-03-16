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
#include <hugo_utils.h>

#define INA1 PIN_PA5
#define INB1 PIN_PB2
#define INA2 PIN_PA3
#define INB2 PIN_PA4

#define I2C_STEP_MOTOR_SET_TARGET_POS 1
#define I2C_STEP_MOTOR_GET_ACTUAL_POS 2
#define I2C_STEP_MOTOR_STEPS_PER_TURN 3
#define I2C_STEP_MOTOR_RESET 4
#define I2C_STEP_MOTOR_CALIBRATE 5
#define I2C_STEP_MOTOR_CALIBRATION_DONE 6


#define STEPS_PER_TURN 500 * 4

uint8_t step_time_ms = 2;
volatile int16_t actual_pos = 0;
volatile int16_t target_pos = 0; //500* 4;
volatile int8_t substep=0;

void move(){
  switch(substep){
    case 0:
      digitalWrite(INA1, LOW);
      digitalWrite(INB1, HIGH);
      digitalWrite(INA2, LOW);
      digitalWrite(INB2, LOW);
    break;
    case 1:
      digitalWrite(INA1, LOW);
      digitalWrite(INB1, LOW);
      digitalWrite(INA2, HIGH);
      digitalWrite(INB2, LOW);
    break;
    case 2:
      digitalWrite(INA1, HIGH);
      digitalWrite(INB1, LOW);
      digitalWrite(INA2, LOW);
      digitalWrite(INB2, LOW);
    break;
    case 3:
      digitalWrite(INA1, LOW);
      digitalWrite(INB1, LOW);
      digitalWrite(INA2, LOW);
      digitalWrite(INB2, HIGH);
    break;
  }
}
void step_clockwise(){
  move();
  substep++;
  if (substep > 3){
    substep = 0;
  }
  actual_pos++;
}

void step_anti_clockwise(){
  move();
  substep--;
  if (substep < 0){
    substep = 3;
  }
  actual_pos--;
}

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){
  case I2C_STEP_MOTOR_SET_TARGET_POS:
    {
      int16_t value = HugoTinyWireRead() << 8;
      value += HugoTinyWireRead();
      noInterrupts();
      target_pos = value;
      interrupts();
    }
    break;
  case I2C_STEP_MOTOR_GET_ACTUAL_POS:
    noInterrupts();
    s_buffer.data[0] = actual_pos >> 8;
    s_buffer.data[1] = actual_pos & 0xFF;
    s_buffer.size = 2;
    interrupts();
    break;
  case I2C_STEP_MOTOR_STEPS_PER_TURN:
    s_buffer.data[0] = STEPS_PER_TURN >> 8;
    s_buffer.data[1] = STEPS_PER_TURN & 0xFF;
    s_buffer.size = 2;
    break;
  case I2C_STEP_MOTOR_RESET:
    noInterrupts();
    actual_pos = 0;
    interrupts();
    break;
  case I2C_STEP_MOTOR_CALIBRATE:
    //TODO:
    break;
  case I2C_STEP_MOTOR_CALIBRATION_DONE:
    s_buffer.data[0] = 0;
    s_buffer.size = 1;
    break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_STEP_MOTOR;
  s_buffer.data[1] = HUGO_PCB_VERSION;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

ISR(RTC_PIT_vect) {
   RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */

  if (target_pos == actual_pos){
    return;
  }
  if (target_pos > actual_pos) {
    step_clockwise();
  }
  else{
    step_anti_clockwise();
  }
}
void setup(){
  RTC_init(RTC_PERIOD_CYC64_gc);
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_STEP_MOTOR, NULL);
  pinMode(INA1, OUTPUT);
  pinMode(INA2, OUTPUT);
  pinMode(INB1, OUTPUT);
  pinMode(INB2, OUTPUT);
}

void loop(){
  sleep_cpu();
}
