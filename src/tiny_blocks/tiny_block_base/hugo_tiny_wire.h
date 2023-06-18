#pragma once

//this function ask setting of power save level
// one of three values is expected for parameter "level":
// no_power_save = 0, light_power_save = 1, deep_power_save = 2
void HugoTinyWirePowerSave(uint8_t level);

// this function processes received i2c command by a particular block - muse be implemented in te .ino file
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);

// returns I2C address of the active extension - muse be implemented in te .ino file
uint8_t HugoTinyWireGetExtAddress();

// set I2C address of the active extension - muse be implemented in te .ino file
void HugoTinyWireChangeExtAddress(uint8_t address);
// defines listening and sending callback functions and starts I2C
// ext_addresses is list tof extension which are list of possible addresses. addresslists are terminated by 0x00
void HugoTinyWireInitialize(uint8_t block_type_id, uint8_t** ext_addresses);

uint8_t HugoTinyWireRead();

// fills module_type + mayor(PCB) version + minor(adjustments) version to s_buffer
// muse be implemented in te .ino file
void HugoTinyWireFillModuleVersion();


//-------implementation-------
//code located in header file to be possible to use conditional compilation (HUGO_TINY_EXTENSIONS)

#include <stdint.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <Wire.h>
#include "hugo_defines.h"

uint8_t s_block_type_id = I2C_BLOCK_TYPE_ID_NONE;
static wire_buffer_t s_buffer;

#if defined(HUGO_TINY_EXTENSIONS) || defined (HUGO_TINY_ONE_EXTENSION)
    static uint8_t** s_ext_addresses = NULL;
    static uint8_t* s_active_extension = NULL;
#endif

#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
    #define POWER_SAVE_PIN PIN_PB1 //FIXME
    bool deepSleepOn = false;
#endif

static void i2c_request_event() {
    if (s_buffer.size > 0){
    Wire.write(s_buffer.data, s_buffer.size);
    s_buffer.size = 0;
  }
}

static void read_unnecessary_data(uint8_t count){
    for (uint8_t counter = 0; counter < count; counter++)
    {
        Wire.read(); //data are not for this type of block, but will be safer to empty the buffer
    }
}
static void i2c_receive_data(int count) {
    if (count < 2){
        return; //not valid command
    }

    uint8_t block_id = Wire.read();
    if (block_id != I2C_BLOCK_TYPE_ID_BASE && block_id != s_block_type_id){
        read_unnecessary_data(count - 1);
        return;
    }

    uint8_t command = Wire.read();
    if (block_id == s_block_type_id){
        HugoTinyWireProcessCommand(command, count - 2);
    }
    else{
        s_buffer.size = 0;
        switch (command){
#if defined(HUGO_TINY_EXTENSIONS)
            case I2C_COMMAND_GET_EXT_COUNT:{
                uint8_t index = 0;
                while (s_ext_addresses[index] != NULL){
                    index++;
                }
                s_buffer.data[0] = index;
                s_buffer.size = 1;
            }
            break;
#elif defined (HUGO_TINY_ONE_EXTENSION)
            case I2C_COMMAND_GET_EXT_COUNT:
                s_buffer.data[0] = 1;
                s_buffer.size = 1;
            break;
#endif
#if defined(HUGO_TINY_EXTENSIONS)
            case I2C_COMMAND_ACTIVATE_EXTENSION:
                s_active_extension = s_ext_addresses[Wire.read()]
            break;
#endif
#if defined(HUGO_TINY_EXTENSIONS) || defined (HUGO_TINY_ONE_EXTENSION)
            case I2C_COMMAND_GET_EXT_ADDRESS:
                s_buffer.data[0] = HugoTinyWireGetExtAddress();
                s_buffer.size = 1;
            break;

            case I2C_COMMAND_CHANGE_EXT_ADDRESS:{
                uint8_t new_address = Wire.read();
                HugoTinyWireChangeExtAddress(new_address);
            }
            break;

            case I2C_COMMAND_GET_EXT_ADDR_COUNT:{
                uint8_t index = 0;
                while (s_active_extension[index] != 0x00){
                    index++;
                }
                s_buffer.data[0] = index;
                s_buffer.size = 1;

            }
            break;

            case I2C_COMMAND_GET_EXT_ADDR_LIST:{
                s_buffer.size = 0;
                while (s_active_extension[s_buffer.size] != 0x00){
                    s_buffer.data[s_buffer.size] = s_active_extension[s_buffer.size];
                    s_buffer.size++;
                }
            }
            break;
#endif
            case I2C_COMMAND_CHANGE_I2C_ADDRESS:
                if (count  > 2){
                    uint8_t new_address = Wire.read();
                    EEPROM.write(EEPROM_I2C_ADDRESS_POS, new_address);

                    // workaround standard wire do not allow to change i2c address
                    // it seems avr do not have an instruction for soft reset reset van be done via watchdog
                    // minimal WTD timeout is 15 milisec
                    wdt_enable(WDTO_15MS);
                    while(1){};

                }
            break;
            case I2C_COMMAND_GET_MODULE_VERSION:
                HugoTinyWireFillModuleVersion();
                break;
            case I2C_COMMAND_SET_POWER_SAVE:
            {
                uint8_t level = Wire.read();
                #if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
                    if (level == POWER_SAVE_DEEP){
                        digitalWrite(POWER_SAVE_PIN, 0);
                        deepSleepOn = true;
                    }
                    else{
                        if (level == POWER_SAVE_NONE and deepSleepOn){
                            digitalWrite(POWER_SAVE_PIN, 1);
                            deepSleepOn = false;
                        }
                        else{
                            HugoTinyWirePowerSave(level);
                        }
                    }
                #else
                    HugoTinyWirePowerSave(level);
                #endif
            }
            break;
            default:
                read_unnecessary_data(count - 2); //to old blocks are not bricked due to missaligned data in case there will be added new command with data
        }
    }
}

void HugoTinyWireInitialize(uint8_t block_type_id, uint8_t** ext_addresses){
    s_block_type_id = block_type_id;
    s_buffer.size = 0;
#if defined(HUGO_TINY_EXTENSIONS) || defined (HUGO_TINY_ONE_EXTENSION)
    s_ext_addresses = ext_addresses;
    s_active_extension = ext_addresses[0];
#endif
    uint8_t address = EEPROM.read(EEPROM_I2C_ADDRESS_POS);
    if (address == EEPROM_DEFAULT_VALUE){
        address =  block_type_id; //block type IDs are chosen to be possible to use them as default I2c address
    }

#if defined(__AVR_ATtiny414__) || defined(__AVR_ATtiny1614__)
    pinMode(POWER_SAVE_PIN, OUTPUT);
    digitalWrite(POWER_SAVE_PIN, 1);


    PORTMUX.CTRLB |= PORTMUX_TWI0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
#endif

     Wire.begin(address);
     Wire.onReceive(i2c_receive_data);
     Wire.onRequest(i2c_request_event);
}

uint8_t HugoTinyWireRead() {
    return Wire.read();
}
