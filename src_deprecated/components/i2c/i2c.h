/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#pragma once
#include <stdbool.h>

bool hugo_i2c_init(int sda_pin, int scl_pin);

/**
 * address - I2C address,
 * block_type_id - defined type of tiny_block to ensured that a command will not be performed by mistake,
 * command - is expected different than I2C_COMMAND_NONE (0xFF),
 * out_data - outgoing data,
 * out_data_size - outgoing data size,
 * in_data - incomming data,
 * in_data_size - incomming data size
 */
bool hugo_i2c_tiny(
    uint8_t address, uint8_t block_type_id, uint8_t command, uint8_t *out_data, uint8_t out_data_size, uint8_t *in_data, uint8_t in_data_size);

/**
 * for non tiny-block I2C modules sending command and data and receive data
 * if there is no command to send,  I2C_COMMAND_NONE must be netered
 * if there is no data to send
 */
bool hugo_i2c(uint8_t address, uint8_t command, uint8_t *out_data, unsigned out_data_size, uint8_t *in_data, unsigned in_data_size);

bool hugo_i2c_init(int sda_pin, int scl_pin);
