#pragma once

#include <stdint.h>

void wakeup();
void process_complete_buffer();
void readJdy();
void set_expected_resp_count(uint8_t count, uint16_t timeout_ms);
void init_ble();
void ble_main();
void process_ble_command(uint8_t command, uint8_t payload_size);
void power_save(uint8_t level);
