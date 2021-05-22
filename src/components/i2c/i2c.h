/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#pragma once
#include <stdbool.h>

bool hugo_i2c_init(int sda_pin, int scl_pin);
bool hugo_i2c_write_command_with_data(int address, uint8_t command, uint8_t *data, size_t data_size);
bool hugo_i2c_write_command(int address, uint8_t command);
bool hugo_i2c_write_data(int address, uint8_t * data, size_t size);
bool hugo_i2c_read_data(int address, uint8_t * data, size_t size);
