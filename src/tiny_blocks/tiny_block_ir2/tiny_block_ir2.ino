/*
BOARD: hugo_adapter + hugo_rgb
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();


#include <hugo_tiny_wire.h>
#include <Wire.h>
#include <avr/sleep.h>
//#include <IRLremote.h>
//#include "Arduino-IRremote/src/TinyIRReceiver.cpp.h"

#define __AVR_ATtiny1616__
#define IR_INPUT_PIN PIN_PA6
#include "TinyIRReceiver.hpp"

#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif 

//#define DECODE_NEC
//#include <IRremote.hpp>


//#define IR_RECV_PIN PIN_PA6


#define I2C_COMMAND_GET_IR_DATA_READY    1
#define I2C_COMMAND_GET_IR_DATA         2

//static struct ir_data_t {
//  uint16_t address;
//  uint8_t command;
//  bool repeat;
//} s_ir_data;

//CNec IRLremote;

static uint16_t ir_address = 10;
static uint8_t ir_command = 20;
static bool ir_repeat = false;
static bool ir_data_ready = true;
  
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command){
  case I2C_COMMAND_GET_IR_DATA_READY:
    s_buffer.data[0] = ir_data_ready;
    s_buffer.size = 1;
    break;
  case I2C_COMMAND_GET_IR_DATA:
    s_buffer.data[0] = ir_address & 0xff;
    s_buffer.data[1] = ir_address >> 8;
    s_buffer.data[2] = ir_command;
    s_buffer.data[3] = ir_repeat;
    s_buffer.size = 4;
    ir_data_ready = false;
    
    //Nec_data_t nec_data = IRLremote.read();
    //s_buffer.data[0] = nec_data.command;
    //s_buffer.data[1] = nec_data.address & 0xff;
    //s_buffer.data[2] = nec_data.address >> 8;
    //s_buffer.size = 3;
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
  //IRLremote.begin(IR_RECV_PIN);
  //IrReceiver.begin(IR_RECV_PIN, false);
  //s_ir_data.command = 0;
  initPCIInterruptForTinyReceiver();
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_IR, NULL);
}

void loop(){
  
  //if (IrReceiver.decode()) {
  //    uint16_t data = IrReceiver.decodedIRData.decodedRawData;
  //    //IrReceiver.printIRResultShort(&Serial); // optional use new print version
      
  //    IrReceiver.resume(); // Enable receiving of the next value
  //}
  
  //delayMicroseconds(100);
}

void handleReceivedTinyIRData(uint16_t aAddress, uint8_t aCommand, bool isRepeat) {
  if (!ir_data_ready){
    ir_address = aAddress;
    ir_command = aCommand;
    ir_repeat = isRepeat;
    ir_data_ready=true;
  }
}
