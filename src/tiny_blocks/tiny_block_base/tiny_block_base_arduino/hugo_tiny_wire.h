#pragma once
#include <stdint.h>

typedef struct wire_buffer_t {
    uint8_t data[16]; //not big amount of data is expected 16B should be more than enough
    uint8_t size;
};

//this function processes receiverd i2c command by a particular block - muse be implemented by the including file
void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);

//defines listening and sending callback functions and starts I2C
void HugoTinyWireInitialize(uint8_t block_type_id);

uint8_t HugoTinyWireRead();
wire_buffer_t* HugoTinyGetBuffer();
