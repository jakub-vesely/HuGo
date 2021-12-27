#pragma once

#define EEPROM_DEFAULT_VALUE 0xFF
#define EEPROM_RESERVED_MAX 0x05 //currently used 1 byte for I2C address
#define EEPROM_I2C_ADDRESS_POS 0x01

#define I2C_ADDRESS_NONE 0xFF

#define I2C_COMMAND_GET_MODULE_VERSION  0xF7
#define I2C_COMMAND_ACTIVATE_EXTENSION  0xF8
#define I2C_COMMAND_GET_EXT_COUNT       0xF9
#define I2C_COMMAND_GET_EXT_ADDRESS     0xFA
#define I2C_COMMAND_CHANGE_EXT_ADDRESS  0xFB
#define I2C_COMMAND_GET_EXT_ADDR_COUNT  0xFC
#define I2C_COMMAND_GET_EXT_ADDR_LIST   0xFD
#define I2C_COMMAND_CHANGE_I2C_ADDRESS  0xFE
#define I2C_COMMAND_NONE                0xFF

//block Id is also used as a default i2c address
#define I2C_BLOCK_TYPE_ID_POWER         0x08
#define I2C_BLOCK_TYPE_ID_RGB           0x09
#define I2C_BLOCK_TYPE_ID_MOTOR_DRIVER  0x0A
#define I2C_BLOCK_TYPE_ID_DISPLAY       0x0B
#define I2C_BLOCK_TYPE_ID_SOUND         0x0C
#define I2C_BLOCK_TYPE_ID_BUTTON        0x0D
#define I2C_BLOCK_TYPE_ID_POSITION      0x0E
#define I2C_BLOCK_TYPE_ID_AMBIENT       0x0F

//#define I2C_BLOCK_TYPE_ID_IR            0x10

#define I2C_BLOCK_TYPE_ID_BASE          0xFA //different from CHANGE_I2C_ADDRESS command
#define I2C_BLOCK_TYPE_ID_NONE          0xFF

typedef struct wire_buffer_t {
    uint8_t data[16]; //not big amount of data is expected 16B should be more than enough
    uint8_t size;
};
