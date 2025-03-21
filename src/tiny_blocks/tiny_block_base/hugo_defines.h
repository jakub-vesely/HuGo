#pragma once

#define EEPROM_DEFAULT_VALUE 0xFF
#define EEPROM_RESERVED_MAX 0x05 //currently used 1 byte for I2C address
#define EEPROM_I2C_ADDRESS_POS 0x01

#define I2C_ADDRESS_NONE 0xFF
#define I2C_ADDRESS_BROADCAST 0x00

//RJ12
#define I2C_COMMAND_RJ12_NO_COMMAND                 0x00

#define I2C_COMMAND_RJ12_PIN5_SET_AS_IN             0x01
#define I2C_COMMAND_RJ12_PIN5_SET_AS_OUT            0x02

#define I2C_COMMAND_RJ12_PIN5_SET_DIG_VALUE         0x11
#define I2C_COMMAND_RJ12_PIN5_GET_DIG_VALUE         0x12
#define I2C_COMMAND_RJ12_PIN5_GET_ANA_VALUE         0x13

#define I2C_COMMAND_RJ12_PIN4_SET_AS_IN             0x21
#define I2C_COMMAND_RJ12_PIN4_SET_AS_OUT            0x22
#define I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_RISI   0x23
#define I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_FALL   0x24
#define I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_CHNG   0x25
#define I2C_COMMAND_RJ12_PIN4_SET_OSCIL_PERIOD_MS   0x26

#define I2C_COMMAND_RJ12_PIN4_SET_DIG_VALUE         0x31
#define I2C_COMMAND_RJ12_PIN4_GET_DIG_VALUE         0x32
#define I2C_COMMAND_RJ12_PIN4_GET_ANA_VALUE         0x33
#define I2C_COMMAND_RJ12_PIN4_GET_COUNT_AND_RESET   0x34
#define I2C_COMMAND_RJ12_PIN4_GET_LAST_TIMESTAMPS   0x35

//common
#define I2C_COMMAND_SET_POWER_SAVE          0xF6
#define I2C_COMMAND_GET_MODULE_VERSION      0xF7
#define I2C_COMMAND_ACTIVATE_EXTENSION      0xF8
#define I2C_COMMAND_GET_EXT_COUNT           0xF9
#define I2C_COMMAND_GET_EXT_ADDRESS         0xFA
#define I2C_COMMAND_CHANGE_EXT_ADDRESS      0xFB
#define I2C_COMMAND_GET_EXT_ADDR_COUNT      0xFC
#define I2C_COMMAND_GET_EXT_ADDR_LIST       0xFD
#define I2C_COMMAND_CHANGE_I2C_ADDRESS      0xFE
#define I2C_COMMAND_NONE                    0xFF

//block Id is also used as a default i2c address
#define I2C_BLOCK_TYPE_ID_POWER             0x08
#define I2C_BLOCK_TYPE_ID_RGB               0x09
#define I2C_BLOCK_TYPE_ID_MOTOR_DRIVER      0x0A
#define I2C_BLOCK_TYPE_ID_DISPLAY           0x0B
#define I2C_BLOCK_TYPE_ID_SOUND             0x0C
#define I2C_BLOCK_TYPE_ID_BUTTON            0x0D
#define I2C_BLOCK_TYPE_ID_DISTANCE          0x0E
#define I2C_BLOCK_TYPE_ID_MOTION_TRACKING   0x0F
#define I2C_BLOCK_TYPE_ID_IR                0x10
#define I2C_BLOCK_TYPE_ID_AMBIENT           0x11
#define I2C_BLOCK_TYPE_ID_BLE               0x12
#define I2C_BLOCK_TYPE_ID_RJ12              0x13
#define I2C_BLOCK_TYPE_ID_SERVO             0x14
#define I2C_BLOCK_TYPE_ID_STEP_MOTOR        0x15

#define I2C_BLOCK_TYPE_ID_BASE          0xFA //different from CHANGE_I2C_ADDRESS command
#define I2C_BLOCK_TYPE_ID_NONE          0xFF

#define POWER_SAVE_NONE     0
#define POWER_SAVE_LIGHT    1
#define POWER_SAVE_DEEP     2

#include <stdint.h>
//#include <tiny_main_base.h>

//ble command resp up to 13 (1B size + 12B answer_data)
//mesh_response up to 15 (1B size) + (2B sender + 12B mesh_data)
//~~mesh_command up to 24 (AT+MESH + 1B CMD + 2B target + 12B mesh data + \r\n)~~ mesh command is never filled to the buffer
#define WIRE_BUFFER_SIZE 16

typedef struct wire_buffer_t {
    uint8_t data[WIRE_BUFFER_SIZE];
    uint8_t size;
} wire_buffer_t;
