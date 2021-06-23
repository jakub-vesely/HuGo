#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct display_adapter_t display_adapter_t;
struct display_adapter_t{
    uint8_t width;
    uint8_t height;
    uint8_t address;
    uint16_t buffer_size;
    uint8_t *buffer;
    bool invert;
    bool initialized;

    void (*init)(display_adapter_t *adapter);
    void (*clean)(display_adapter_t *adapter);
    bool (*set_point)(display_adapter_t *adapter, int x, int y, bool color);
    bool (*get_point)(display_adapter_t *adapter, int x, int y);
    void (*showtime)(display_adapter_t *adapter);
};
