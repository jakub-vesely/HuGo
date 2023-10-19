#pragma once
#include <stdint.h>

enum Rj12PinId {
    pin4,
    pin5
};

enum Rj12PinMode {
    input,
    output
};

void tiny_main_rj12_set_pin_mode(uint8_t block_address, Rj12PinId pin, Rj12PinMode mode);
void tiny_main_rj12_set_pin_value(uint8_t block_address, Rj12PinId pin, bool value);
uint16_t tiny_main_rj12_get_pin_value_analog(uint8_t block_address, Rj12PinId pin);
bool tiny_main_rj12_get_pin_value_digital(uint8_t block_address, Rj12PinId pin);
