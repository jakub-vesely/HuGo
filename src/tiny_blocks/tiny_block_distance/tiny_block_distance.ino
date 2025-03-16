/*
Copyright (c) 2022 Jakub Vesely
This software is published under MIT license. Full text of the license is available at https://opensource.org/licenses/MIT

VL53L1 distance measurement block

BOARD: hugo_adapter + proto + VL53L1
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

#define WIDTH   64
#define HEIGHT  48

//#define EEPROM_A0_POS EEPROM_RESERVED_MAX + 1

#define XSHUT_PIN HUGO_PIN_D3

#define EXT_ADDRESS_DEFAULT   0x29
#define EXT_ADDRESS_PRESET1   0x28
#define EXT_ADDRESS_PRESET2   0x27
#define EXT_ADDRESS_PRESET3   0x26

static const uint8_t ext1_addresses[5] = { EXT_ADDRESS_DEFAULT, EXT_ADDRESS_PRESET1, EXT_ADDRESS_PRESET2, EXT_ADDRESS_PRESET3, 0x00 };
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
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_DISTANCE, (uint8_t**)ext_addresses);

  pinMode(XSHUT_PIN, OUTPUT);
  digitalWrite(XSHUT_PIN, 1);
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_DISTANCE;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
    if (level == POWER_SAVE_NONE){
        digitalWrite(XSHUT_PIN, 1);
    }
    else{
        digitalWrite(XSHUT_PIN, 0);
    }
}

void loop() {
  delayMicroseconds(10);
}
