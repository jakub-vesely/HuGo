#include "hugo_tiny_block_base_arduino.h"
#include "hugo_tiny_block_defines.h"
#include <EEPROM.h>
#include <Wire.h>

#define EEPROM_I2C_ADDRESSS_POS 0x01

#define I2C_DEFAULT_VALUE 0x02

uint8_t HugoTinyBlockBaseArduino::GetI2cAddress(){
    uint8_t address = EEPROM.read(EEPROM_I2C_ADDRESSS_POS);
    if (address == EEPROM_DEFAULT_VALUE){
        return I2C_DEFAULT_VALUE;
    }
    return address;
}

void HugoTinyBlockBaseArduino::ReceiveBaseI2cData(uint8_t command, uint8_t payload_length, uint8_t* out_data){
    switch (command){
        case I2C_COMMAND_CHANGE_I2C_ADDRESS:
            if (payload_length  > 0){
                uint8_t new_address = Wire.read();
                EEPROM.write(EEPROM_I2C_ADDRESSS_POS, new_address);
                Wire.end();
                Wire.begin(new_address);
            }
            break;
    }
}