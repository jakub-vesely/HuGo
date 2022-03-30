/*
BOARD: hugo_adapter + hugo_sound
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>

#define PCB_VERSION 2
#define ADJUSTMENT_VERSION 0

//writitn to and reading ffrom serial line is asynchronous but is not performed while i2c is under processing 
//there must be two commands and they have to be send separatelly with delay to be data already prepared in the Serial buffer
#define I2C_COMMAND_WT_CHIP_COMMAND   0x01
#define I2C_COMMAND_WT_CHIP_RESPONSE   0x02

#define COMMAND_PREFIX 0x7e
#define COMMAND_SUFFIX 0xef

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_WT_CHIP_COMMAND: {
      int incoming;
      do{
        incoming = Serial.read();
      }
      while (incoming != -1); //clean buffer
        
      uint8_t pos = 0;
      s_buffer.data[pos++] = COMMAND_PREFIX;
    
      uint8_t length = payload_size + 2;//added check_code + end_code
      s_buffer.data[pos++] = length; // command + payload + check_code + end_code
      uint8_t command_id =  HugoTinyWireRead();
      s_buffer.data[pos++] = command_id;

      uint8_t check_code = length;
      check_code += command_id;
      
      for (uint8_t i = 0; i < payload_size - 1; i++){ //removes command_id
        s_buffer.data[pos] = HugoTinyWireRead(); //reuse s_buffer
        check_code += s_buffer.data[pos++];
      }

      s_buffer.data[pos++] = check_code;
      s_buffer.data[pos++] = COMMAND_SUFFIX;
      Serial.write(s_buffer.data, pos);
      break;
    }
    case I2C_COMMAND_WT_CHIP_RESPONSE: {
      uint8_t expected_size = HugoTinyWireRead(); 
      s_buffer.size = 0;
      if (expected_size > 0) {  
        int incoming;
        while (s_buffer.size < expected_size){
          incoming = s_buffer.data[s_buffer.size++] = Serial.read(); 
        }
      }
      break;
    }
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_SOUND;
  s_buffer.data[1] = PCB_VERSION;
  s_buffer.data[2] = ADJUSTMENT_VERSION;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup()
{
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_SOUND, NULL);

  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop()
{
}
