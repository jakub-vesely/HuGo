/*
power board management

BOARD: hugo_adapter + hugo_power
Chip: ATtiny412
Clock Speed: 5MHz
Programmer: jtag2updi (megaTinyCore)
*/
#include <stdint.h>
#include <Wire.h>
#include <avr/sleep.h>
#include <EEPROM.h>
#include <hugo_tiny_block_defines.h>
#include "tiny_block_power_arduino.h"

#define EEPROM_A0A1_POS EEPROM_RESERVED_MAX + 1
#define INA_I2C_ADDRESS_NA0_NA1 0x40
#define INA_I2C_ADDRESS_A0_NA1  0x41
#define INA_I2C_ADDRESS_NA0_A1  0x44
#define INA_I2C_ADDRESS_A0_A1   0x45 //should be the default one

#define INA_A0_PIN        PIN_PA7
#define INA_A1_PIN        PIN_PA6
#define CHARGE_STATE_PIN  PIN_PA3

static uint8_t s_data;
static uint8_t s_ina_a; //two lowest bites only are valid (a0 lowest, a1, second lowest)
static HugoTinyBlockBaseArduino block(I2C_BLOCK_TYPE_ID_POWER);

uint8_t _get_ina_address() {
  uint8_t a0 = s_ina_a & 1;
  uint8_t a1 = (s_ina_a >> 1) & 1;

  if (a0 && !a1){
    return INA_I2C_ADDRESS_A0_NA1;
  }
  else if (!a0 && a1){
    return INA_I2C_ADDRESS_NA0_A1;
  }
  else if (a0 && a1){
    return INA_I2C_ADDRESS_A0_A1;
  }
  else{ //!a0 && !a1
    return INA_I2C_ADDRESS_NA0_NA1;
  }
}

void i2c_receive_data(int count) {
  uint8_t command = block.ReceiveI2cCommand(count);
  switch (command){
    case I2C_COMMAND_POWER_GET_CHARGING_STATE:
      s_data = (digitalRead(CHARGE_STATE_PIN) == HIGH ? LOW : HIGH); //pin is set to LOW when charging
      break;

    case I2C_COMMAND_POWER_SET_INA_A0A1:
      if (block.GetI2cPayloadSize(count) > 0){
        s_ina_a = Wire.read();
        EEPROM.write(EEPROM_A0A1_POS, s_ina_a);
        set_ina_a_pins();
      }
      break;
    case I2C_COMMAND_POWER_GET_INA_I2C_ADDRESS:
      s_data = _get_ina_address();
      break;
  }
}

void i2c_request_event() {
  Wire.write(s_data);
}

void set_ina_a_pins(){
  digitalWrite(INA_A0_PIN, s_ina_a & 1);
  digitalWrite(INA_A1_PIN, (s_ina_a >> 1) & 1);
}

void setup(){
  pinMode(INA_A0_PIN, OUTPUT);
  pinMode(INA_A1_PIN, OUTPUT);
  pinMode(CHARGE_STATE_PIN, INPUT);

  s_ina_a = EEPROM.read(EEPROM_A0A1_POS); //default value should be 0xFF
  set_ina_a_pins();

  Wire.begin(block.GetI2cAddress());
  Wire.onReceive(i2c_receive_data);
  Wire.onRequest(i2c_request_event);
}

void loop(){
  delayMicroseconds(500);
}
