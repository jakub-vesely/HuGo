/*
power board management

BOARD: hugo_adapter + hugo_ir
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <Wire.h>
#include <avr/sleep.h>
#include <hugo_tiny_block_base.h>
#include "tiny_block_ir_arduino.h"
#include <IRLremote.h>
//#include "Arduino-IRremote/src/TinyIRReceiver.cpp.h"

//#define IR_INPUT_PIN PIN_PA6
#define IR_RECV_PIN PIN_PA6

static struct ir_data_t {
  uint16_t address;
  uint8_t command;
  bool repeat;
} s_ir_data;

static HugoTinyBlockBaseArduino blockBase(I2C_BLOCK_TYPE_ID_IR);
CNec IRLremote;

void i2c_receive_data(int count) {
  s_data_size = 0;
  uint8_t command = blockBase.ReceiveI2cCommand(count);
  switch (command){
  case I2C_COMMAND_GET_IR_DATA_READY:
    s_data[0] = IRLremote.available();
    //s_data[0] = s_ir_data.command != 0;
    s_data_size = 1;
    break;
  case I2C_COMMAND_GET_IR_DATA:
    Nec_data_t nec_data = IRLremote.read();
    s_data[0] = nec_data.command;
    s_data[1] = nec_data.address & 0xff;
    s_data[2] = nec_data.address >> 8;
    s_data_size = 3;
    // s_data[0] = s_ir_data.command;
    // s_data[1] = s_ir_data.address & 0xff;
    // s_data[2] = s_ir_data.address >> 8;
    // s_data_size = 3;
    //s_ir_data.command = 0;
    break;
  }
}

void setup(){
  IRLremote.begin(IR_RECV_PIN);
  s_ir_data.command = 0;

  //initPCIInterruptForTinyReceiver();
}

void loop(){
  delayMicroseconds(100);
}
/*
void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) {
  s_ir_data.address = aAddress;
  s_ir_data.repeat = isRepeat;
  s_ir_data.command = aCommand;
}
*/