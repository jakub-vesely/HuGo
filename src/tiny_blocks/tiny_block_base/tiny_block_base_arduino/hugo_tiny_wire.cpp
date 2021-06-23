#include "hugo_tiny_wire.h"
#include "hugo_tiny_block_defines.h"
#include <EEPROM.h>
#include <Wire.h>

#define EEPROM_I2C_ADDRESSS_POS 0x01
#define I2C_DEFAULT_VALUE 0x02

uint8_t s_block_type_id = I2C_BLOCK_TYPE_ID_NONE;
static wire_buffer_t s_buffer;

static void i2c_request_event() {
    if (s_buffer.size > 0){
    Wire.write(s_buffer.data, s_buffer.size);
    s_buffer.size = 0;
  }
}

wire_buffer_t* HugoTinyGetBuffer(){
    return &s_buffer;
}

void serialEvent() {
    uint8_t block_id = 0;
    uint8_t command = 0;
    if (Serial.available()){
        block_id = Serial.read();
    }

    if (Serial.available()){
        command = Serial.read();
    }

}
static void i2c_receive_data(int count) {
    if (count < 2){
        return; //not valid command
    }

    uint8_t block_id = Wire.read();
    if (block_id != I2C_BLOCK_TYPE_ID_BASE && block_id != s_block_type_id){
        for (uint8_t counter = 0; counter < count - 1; counter++)
        {
            Wire.read(); //data are not for this type of block, but will be safer to empty the buffer
        }
        return;
    }

    uint8_t command = Wire.read();
    if (block_id == s_block_type_id){
        HugoTinyWireProcessCommand(command, count - 2);
    }
    else{
        switch (command){
            case I2C_COMMAND_CHANGE_I2C_ADDRESS:
                if (count  > 2){
                    uint8_t new_address = Wire.read();
                    EEPROM.write(EEPROM_I2C_ADDRESSS_POS, new_address);
                }
                break;
        }
    }
}

void HugoTinyWireInitialize(uint8_t block_type_id){
    s_block_type_id = block_type_id;
    s_buffer.size = 0;

    uint8_t address = EEPROM.read(EEPROM_I2C_ADDRESSS_POS);
    if (address == EEPROM_DEFAULT_VALUE){
        address =  I2C_DEFAULT_VALUE;
    }

    Wire.begin(address);
    Wire.onReceive(i2c_receive_data);
    Wire.onRequest(i2c_request_event);
}

uint8_t HugoTinyWireRead(){
    return Wire.read();
}