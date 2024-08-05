/*
Copyright (c) 2022 Jakub Vesely
This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

BME280 weather block

PCBs: hugo_adapter + sensor_board be sure that the pin PA7 is not populated on the sensor board (70uA current increase otherwise) PA3 and PA6 coul be also unoccupied
Chip: ATtiny412 (414)
Clock Speed: 1MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
uint8_t HugoTinyWireGetExtAddress();
void HugoTinyWireChangeExtAddress(uint8_t address);
void HugoTinyWireFillModuleVersion();

#define HUGO_TINY_ONE_EXTENSION

#include <EEPROM.h>

#include <hugo_tiny_wire.h>
#include <hugo_defines.h>

#define EXT_ADDRESS_DEFAULT 0x76
#define EXT_ADDRESS_SDO_HI  0x77


static const uint8_t ext1_addresses[5] = { EXT_ADDRESS_DEFAULT, EXT_ADDRESS_SDO_HI, 0x00 };
static const uint8_t* ext_addresses[2] = { (uint8_t*)ext1_addresses, NULL };

static uint8_t s_current_ext_address = EXT_ADDRESS_DEFAULT;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
}

uint8_t HugoTinyWireGetExtAddress(){
  return s_current_ext_address;
}

void HugoTinyWireChangeExtAddress( uint8_t address){
      //TODO
}

void setup(){
  //pinMode(XSHUT_PIN, OUTPUT);
  //digitalWrite(XSHUT_PIN, 1);


  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_AMBIENT, (uint8_t**)ext_addresses);
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_AMBIENT;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
  //sleep procedures in tiny_wire
}

void loop() {
  sleep_cpu(); //functionality is driven by i2c commands
}
