#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <display_adapter.h>

display_adapter_t *hugo_ssd1306_init(uint8_t i2c_address, uint8_t width, uint8_t height, bool invert);
