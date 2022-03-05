/*
BOARD: hugo_adapter + hugo_rgb
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>

#include <IRLremote.h>

#define IR_RECV_PIN PIN_PA6

#define I2C_COMMAND_GET_IR_DATA_READY    1
#define I2C_COMMAND_GET_IR_DATA         2

static struct ir_data_t {
  uint16_t address;
  uint8_t command;
  bool repeat;
} s_ir_data;

CNec IRLremote;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){
  case I2C_COMMAND_GET_IR_DATA_READY:
    s_buffer.data[0] = IRLremote.available();
    s_buffer.size = 1;
    break;
  case I2C_COMMAND_GET_IR_DATA:
    Nec_data_t nec_data = IRLremote.read();
    uint16_t ir_address = nec_data.address;
    uint8_t ir_command = nec_data.command;

    if (ir_address == 0xffff){
      s_ir_data.repeat = true;
    }
    else{
      s_ir_data.address = ir_address;
      s_ir_data.command = ir_command;
      s_ir_data.repeat = false;
    }
    
    s_buffer.data[0] = s_ir_data.address & 0xff;
    s_buffer.data[1] = s_ir_data.address >> 8;
    s_buffer.data[2] = s_ir_data.command;
    s_buffer.data[3] = s_ir_data.repeat;
    s_buffer.size = 4;
    break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_IR;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup(){
  IRLremote.begin(IR_RECV_PIN);
  s_ir_data.command = 0;

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_IR, NULL);
}

void loop(){
  //delayMicroseconds(100);
}
