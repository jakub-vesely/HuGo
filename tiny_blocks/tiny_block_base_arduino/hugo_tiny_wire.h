#pragma once
#include <stdint.h>

typedef struct wire_buffer_t {
    uint8_t data[16]; //not big amount of data is expected 16B should be more than enough
    uint8_t size;
};

// this function processes received i2c command by a particular block - muse be implemented in te .ino file
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);

// returns I2C address of extension - muse be implemented in te .ino file
// return address of ext module defined by index
uint8_t HugoTinyWireGetExtAddress(uint8_t extension_index);

// set I2C address of extension - muse be implemented in te .ino file
// returns true if successfully set
bool HugoTinyWireSetExtAddress(uint8_t extension_index, uint8_t address);

// defines listening and sending callback functions and starts I2C
// ext_addresses is list tof extension which are list of possible addresses. addresslists are terminated by 0x00
void HugoTinyWireInitialize(uint8_t block_type_id, uint8_t* ext_addresses);

uint8_t HugoTinyWireRead();
wire_buffer_t* HugoTinyGetBuffer();
