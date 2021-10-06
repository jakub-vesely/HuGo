#define EEPROM_DEFAULT_VALUE 0xFF
#define EEPROM_RESERVED_MAX 0x05 //currently used 1 byte for I2C address

#define I2C_ADDRESS_NONE                0xFF

#define I2C_COMMAND_GET_EXT_COUNT       0xF9
#define I2C_COMMAND_GET_EXT_ADDRESS     0xFA
#define I2C_COMMAND_CHANGE_EXT_ADDRESS  0xFB
#define I2C_COMMAND_GET_EXT_ADDR_COUNT  0xFC
#define I2C_COMMAND_GET_EXT_ADDR_LIST   0xFD
#define I2C_COMMAND_CHANGE_I2C_ADDRESS  0xFE
#define I2C_COMMAND_NONE                0xFF

#define I2C_BLOCK_TYPE_ID_POWER         0x01
#define I2C_BLOCK_TYPE_ID_RGB           0x02
#define I2C_BLOCK_TYPE_ID_MOTOR_DRVR    0x03
#define I2C_BLOCK_TYPE_ID_IR            0x04
#define I2C_BLOCK_TYPE_ID_DISPLAY       0x05

#define I2C_BLOCK_TYPE_ID_BASE          0xFA //diferent from CHANGE_I2C_ADDRESS command
#define I2C_BLOCK_TYPE_ID_NONE          0xFF
