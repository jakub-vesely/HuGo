#pragma once


// this function processes received i2c command by a particular block - muse be implemented in te .ino file
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);

// returns I2C address of extension - muse be implemented in te .ino file
// return address of ext module defined by index
uint8_t HugoTinyWireGetExtAddress(uint8_t extension_index);

// set I2C address of extension - muse be implemented in te .ino file
// returns true if successfully set
bool HugoTinyWireChangeExtAddress(uint8_t extension_index, uint8_t address);
// defines listening and sending callback functions and starts I2C
// ext_addresses is list tof extension which are list of possible addresses. addresslists are terminated by 0x00
void HugoTinyWireInitialize(uint8_t block_type_id, uint8_t* ext_addresses, uint8_t ext_addresses_size);

uint8_t HugoTinyWireRead();


//code located in header file to be possible to use conditional compilation (HUGO_TINY_EXTENSIONS)

#include <stdint.h>
#include "hugo_tiny_wire.h"
#include "hugo_tiny_block_defines.h"
#include <EEPROM.h>
#include <Wire.h>

#define EEPROM_I2C_ADDRESSS_POS 0x01
#define I2C_DEFAULT_VALUE 0x02

typedef struct wire_buffer_t {
    uint8_t data[16]; //not big amount of data is expected 16B should be more than enough
    uint8_t size;
};

uint8_t s_block_type_id = I2C_BLOCK_TYPE_ID_NONE;
static wire_buffer_t s_buffer;

#ifdef HUGO_TINY_EXTENSIONS
    static uint8_t* s_ext_addresses = NULL;
    static uint8_t s_ext_addresses_size = 0;
#endif

static void i2c_request_event() {
    if (s_buffer.size > 0){
    Wire.write(s_buffer.data, s_buffer.size);
    s_buffer.size = 0;
  }
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
#ifdef HUGO_TINY_EXTENSIONS
            case I2C_COMMAND_GET_EXT_COUNT:
                s_buffer.data[0] = 0;
                for (uint8_t index = 0; index < s_ext_addresses_size; ++index){
                    if (s_ext_addresses[index] == 0x00){
                        s_buffer.data[0]++;
                    }
                }
                s_buffer.size = 1;
            break;

            case I2C_COMMAND_GET_EXT_ADDRESS:
                s_buffer.data[0] = HugoTinyWireGetExtAddress(Wire.read());
                s_buffer.size = 1;
            break;

            case I2C_COMMAND_CHANGE_EXT_ADDRESS:{
                uint8_t extension_index = Wire.read();
                uint8_t new_address = Wire.read();
                s_buffer.data[0] = HugoTinyWireChangeExtAddress(extension_index, new_address);
                s_buffer.size = 1;
            }
            break;

            case I2C_COMMAND_GET_EXT_ADDR_COUNT:{
                s_buffer.data[0] = 0; //size is the first byte
                s_buffer.size = 1;

                uint8_t order = Wire.read();
                for (int8_t index = 0; index < s_ext_addresses_size; ++index){
                    if (order == 0){
                        if (s_ext_addresses[index] == 0x00){
                            break;
                        }
                        s_buffer.data[0]++;
                    }
                    else if (s_ext_addresses[index] == 0x00){
                        order--;
                    }
                }
            }
            break;

            case I2C_COMMAND_GET_EXT_ADDR_LIST:{
                s_buffer.size = 0;

                uint8_t order = Wire.read();
                for (int8_t index = 0; index < s_ext_addresses_size; ++index){
                    if (order == 0){
                        if (s_ext_addresses[index] == 0x00){
                            break;
                        }
                        s_buffer.data[s_buffer.size++] = s_ext_addresses[index];
                    }
                    else if (s_ext_addresses[index] == 0x00){
                        order--;
                    }
                }
            }
            break;
#endif
            case I2C_COMMAND_CHANGE_I2C_ADDRESS:
                if (count  > 2){
                    uint8_t new_address = Wire.read();
                    EEPROM.write(EEPROM_I2C_ADDRESSS_POS, new_address);
                }
            break;
        }
    }
}

void HugoTinyWireInitialize(uint8_t block_type_id, uint8_t* ext_addresses, uint8_t ext_addresses_size){
    s_block_type_id = block_type_id;
    s_buffer.size = 0;
#ifdef HUGO_TINY_EXTENSIONS
    s_ext_addresses = ext_addresses;
    s_ext_addresses_size = ext_addresses_size;
#endif
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
