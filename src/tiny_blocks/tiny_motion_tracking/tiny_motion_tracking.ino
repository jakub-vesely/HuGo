/*
Copyright (c) 2022 Jakub Vesely
This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

VL53L1 distance measurement block

BOARD: hugo_adapter + sensor + (MPU9250+BMP280)
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/



void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
uint8_t HugoTinyWireGetExtAddress();
void HugoTinyWireChangeExtAddress(uint8_t address);
void HugoTinyWireFillModuleVersion();

#define HUGO_TINY_ONE_EXTENSION

#include <hugo_tiny_wire.h>
#include <hugo_defines.h>
#include <avr/sleep.h>
#include <EEPROM.h>

#define EXT_ADDRESS_MPU9250_AD0_0 0x68
#define EXT_ADDRESS_MPU9250_AD0_1 0x69

static const uint8_t mpu_addresses[3] = { EXT_ADDRESS_MPU9250_AD0_0, EXT_ADDRESS_MPU9250_AD0_1, 0x00 };

static const uint8_t* ext_addresses[2] = { (uint8_t*)mpu_addresses, NULL };

static uint8_t s_current_ext_address = EXT_ADDRESS_MPU9250_AD0_0;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
}

uint8_t HugoTinyWireGetExtAddress(){
  return s_current_ext_address;
}

void HugoTinyWireChangeExtAddress( uint8_t address){
  //TODO
}

void setup(){
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_MOTION_TRACKING, (uint8_t**)ext_addresses);
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_MOTION_TRACKING;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
}

void loop() {
  delayMicroseconds(500);
}
