#pragma once
#include <stdint.h>

#define COMMON_BUFFER_SIZE 30 // biggest is a mesh message (up to 28)
typedef struct tiny_common_buffer_t{
  uint8_t data[COMMON_BUFFER_SIZE];
  uint8_t size;
} tiny_common_buffer_t;

tiny_common_buffer_t* tiny_main_base_get_common_buffer();
void tiny_main_base_init();
void tiny_main_base_set_build_in_led_a(bool state);
void tiny_main_base_set_build_in_led_b(bool state);
bool tiny_main_base_send_i2c_command(uint8_t address, uint8_t block_type_id, uint8_t command, uint8_t expected_response_size);
bool tiny_main_base_send_i2c_message(uint8_t address, uint8_t expected_response_size);
uint8_t tiny_main_base_get_ext_module_address(uint8_t address);


void tiny_main_base_set_power_save(uint8_t address, uint8_t mode);

void tiny_main_string_to_buffer(char const *str);
void tiny_main_base_fill_module_version(uint8_t block_address);
bool tiny_main_base_is_available(uint8_t block_address);
