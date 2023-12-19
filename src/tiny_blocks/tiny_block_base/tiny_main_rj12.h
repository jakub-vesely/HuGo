#pragma once

#include <stdint.h>

enum Rj12PinId {
    pin4,
    pin5
};

enum Rj12PinMode {
    input,
    output,
    interrupt_rising,
    interrupt_falling,
    interrupt_change
};

void tiny_main_rj12_set_pin_mode(uint8_t block_address, Rj12PinId pin, Rj12PinMode mode);
void tiny_main_rj12_set_pin_value(uint8_t block_address, Rj12PinId pin, bool value);
uint16_t tiny_main_rj12_get_pin_value_analog(uint8_t block_address, Rj12PinId pin);
bool tiny_main_rj12_get_pin_value_digital(uint8_t block_address, Rj12PinId pin);
uint16_t tiny_main_rj12_pin5_get_count_and_reset(uint8_t block_address);
bool tiny_main_rj12_pin5_get_timestamp_diffs(uint8_t block_address);
bool tiny_main_rj12_pin5_set_oscil_period_ms(uint8_t block_address, uint16_t period_ms);
