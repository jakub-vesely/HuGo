#pragma once
#include <stdint.h>

class HugoTinyBlockBaseArduino {
public:
    uint8_t GetI2cAddress();
    void ReceiveBaseI2cData(uint8_t command, uint8_t payload_length, uint8_t *out_data);
};
