#pragma once

#include <stdbool.h>
#include "display_adapter.h"

bool hugo_graphics_set_point(display_adapter_t *display, int x0, int y0, bool color);
bool hugo_graphics_get_point(display_adapter_t *display,int x0, int y0);
void hugo_graphics_print_text_8x8(display_adapter_t *display, int x0, int y0, int width, int height, char *text, bool color);
void hugo_graphics_print_text_8x16(display_adapter_t *display, int x0, int y0, int width, int height, char *text, bool color);
void hugo_graphics_draw_elipse(display_adapter_t *display, int x, int y, int r1, int r2, bool color);

