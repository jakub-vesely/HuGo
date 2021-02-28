#pragma once
#include <stdbool.h>

typedef struct {
    int width;
    int height;

    void (*clean)();
    bool (*set_point)(int x, int y, bool color);
    bool (*get_point)(int x, int y);
    void (*showtime)();
} display_adapter_t;
