/*
power board management

BOARD: hugo_adapter + hugo_disp
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <hugo_tiny_wire.h>
#include <avr/sleep.h>
#include <EEPROM.h>
//#include <hugo_tiny_block_base_arduino.h>
#include "tiny_block_display_arduino.h"

#define WIDTH   64
#define HEIGHT  48

#define EEPROM_A0_POS EEPROM_RESERVED_MAX + 1

#define SSD_A0_PIN PIN_PA7

static wire_buffer_t *s_buffer = NULL;
static uint8_t s_ssd_a0; //lowest bite is relevant

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){
    case I2C_COMMAND_DISP_SET_SSD_A0:
      if (payload_size > 0){
        s_ssd_a0 = HugoTinyWireRead();
        EEPROM.write(EEPROM_A0_POS, s_ssd_a0);
        set_ssd_a0_pin();
      }
      break;
    case I2C_COMMAND_DISP_GET_PARAMS:
      s_buffer->data[0] = s_ssd_a0 ? INA_I2C_ADDRESS_A0 : INA_I2C_ADDRESS_NA0;
      s_buffer->data[1] = WIDTH;
      s_buffer->data[2] = HEIGHT;
      s_buffer->size = 3;
      break;
  }
}

void set_ssd_a0_pin(){
  digitalWrite(SSD_A0_PIN, s_ssd_a0 != 0);
}

void setup(){
  s_buffer = HugoTinyGetBuffer();
  pinMode(SSD_A0_PIN, OUTPUT);

  s_ssd_a0 = EEPROM.read(EEPROM_A0_POS); //default value should be 0xFF
  set_ssd_a0_pin();

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_DISPLAY);
}

void loop(){
  delayMicroseconds(500);
}
