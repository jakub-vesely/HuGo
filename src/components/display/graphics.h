#pragma once

#include <stdbool.h>

void hugo_graphics_init(display_adapter_t *display_adapter);
bool hugo_graphics_set_point(int x0, int y0, bool color);
bool hugo_graphics_get_point(int x0, int y0);
void hugo_graphics_print_text_8x8(int x0, int y0, int width, int height, char *text, bool color);
void hugo_graphics_print_text_8x16(int x0, int y0, int width, int height, char *text, bool color);
void hugo_graphics_draw_elipse(int x, int y, int r1, int r2, bool color);

