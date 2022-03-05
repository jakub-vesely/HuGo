/*
Copyright (c) 2022 Jakub Vesely
This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

ssd1306 display block

BOARD: hugo_adapter + hugo_disp
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
#include <avr/sleep.h>
#include <EEPROM.h>

#define WIDTH   64
#define HEIGHT  48

#define EEPROM_A0_POS EEPROM_RESERVED_MAX + 1

#define SSD_A0_PIN PIN_PA7

#define I2C_COMMAND_DISP_GET_DIMENSIONS 0x03

#define EXT_ADDRESS_NA0   0x3c
#define EXT_ADDRESS_A0    0x3d

static const uint8_t ext1_addresses[3] = { EXT_ADDRESS_NA0, EXT_ADDRESS_A0, 0x00 };
static const uint8_t* ext_addresses[2] = { (uint8_t*)ext1_addresses, NULL };

static uint8_t s_current_ext_address = 0;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){
    case I2C_COMMAND_DISP_GET_DIMENSIONS:
      s_buffer.data[0] = WIDTH;
      s_buffer.data[1] = HEIGHT;
      s_buffer.size = 2;
      break;
  }
}

uint8_t HugoTinyWireGetExtAddress(){
  return s_current_ext_address;
}

void HugoTinyWireChangeExtAddress( uint8_t address){
  s_current_ext_address = address;
  uint8_t a0_value = (address == EXT_ADDRESS_A0 ? 1 : 0);
  EEPROM.write(EEPROM_A0_POS, a0_value);
  digitalWrite(SSD_A0_PIN, a0_value);
}

void setup(){
  pinMode(SSD_A0_PIN, OUTPUT);
  uint8_t a0_value = EEPROM.read(EEPROM_A0_POS);  //default value of eeprom should be 0xFF
  s_current_ext_address = a0_value ? EXT_ADDRESS_A0 : EXT_ADDRESS_NA0;
  digitalWrite(SSD_A0_PIN, a0_value);

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_DISPLAY, (uint8_t**)ext_addresses);
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_DISPLAY;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void loop() {
  delayMicroseconds(500);
}
