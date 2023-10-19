/*
BOARD: hugo_adapter + hugo_rj12
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>

#define PCB_VERSION 1
#define ADJUSTMENT_VERSION 1

#define RJ_PIN4 PIN_PA3
#define RJ_PIN5 PIN_PA6

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_RJ12_SET_PIN4_AS_IN:
      pinMode(RJ_PIN4, INPUT_PULLUP);
      break;
    case I2C_COMMAND_RJ12_SET_PIN5_AS_IN:
      pinMode(RJ_PIN5, INPUT_PULLUP);
      break;
    case I2C_COMMAND_RJ12_SET_PIN4_AS_OUT:
      pinMode(RJ_PIN4, OUTPUT);
      break;
    case I2C_COMMAND_RJ12_SET_PIN5_AS_OUT:
      pinMode(RJ_PIN5, OUTPUT);
      break;
    case I2C_COMMAND_RJ12_SET_PIN4_DIGITAL_VALUE:
      if (payload_size > 0){
        digitalWrite(RJ_PIN4, HugoTinyWireRead());
      }
      break;
    case I2C_COMMAND_RJ12_SET_PIN5_DIGITAL_VALUE:
      if (payload_size > 0){
        digitalWrite(RJ_PIN5, HugoTinyWireRead());
      }
      break;
    case I2C_COMMAND_RJ12_GET_PIN4_DIG_VALUE:
      s_buffer.data[0] = digitalRead(RJ_PIN4, HugoTinyWireRead());
      s_buffer.size = 1;
      break;
    case I2C_COMMAND_RJ12_GET_PIN5_DIG_VALUE:
      s_buffer.data[0] = digitalRead(RJ_PIN5, HugoTinyWireRead());
      s_buffer.size = 1;
      break;
    case I2C_COMMAND_RJ12_GET_PIN4_ANA_VALUE:{
      int value = analogRead(RJ_PIN4, HugoTinyWireRead());
      s_buffer.data[0] = value & 0xff;
      s_buffer.data[1] = value >> 8;
      s_buffer.size = 2;
    }
      break;
    case I2C_COMMAND_RJ12_GET_PIN5_ANA_VALUE:
      int value = analogRead(RJ_PIN5, HugoTinyWireRead());
      s_buffer.data[0] = value & 0xff;
      s_buffer.data[1] = value >> 8;
      s_buffer.size = 2;
  }
      break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_RJ12;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
}

void setup()
{
  pinMode(RJ_PIN4, INPUT);
  pinMode(RJ_PIN5, INPUT);

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_RJ12, NULL);
}

void loop()
{
}
