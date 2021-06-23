#include "display_adapter.h"
#include "font8x8.h"
#include "graphics.h"
#include "font8x16.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>

bool hugo_graphics_set_point(display_adapter_t *display, int x0, int y0, bool color)
{
    return display->set_point(display, x0, y0, color);
}

bool hugo_graphics_get_point(display_adapter_t *display, int x0, int y0)
{
    return display->get_point(display, x0, y0);
}

void _print_text(display_adapter_t *display, int x0, int y0, int width, int height, char *text, bool color, uint8_t* font, int char_width, int char_height, bool h_space)
{
    int index = 0;
    int column = 0;
    int row = 0;
    int char_height_byte = char_height / 8;

    while (text[index] != '\0')
    {
        if (text[index] == '\n' || column >= width)
        {
            row += char_height;
            column = 0;
        }
        else
        {
            if (row >= height) break;

            int char_start_pos = text[index] * char_width * char_height_byte;
            for (int char_col = 0; char_col < char_width; ++char_col)
            {
                for (int char_byte = 0; char_byte < char_height_byte; ++char_byte)
                {
                    uint8_t col_value = font[char_start_pos + char_col * char_height_byte + char_byte];
                    for (int char_row = 0; char_row < 8; ++char_row)
                    {
                        bool point_color = ((col_value >> char_row) & 1) ? color : !color;
                        display->set_point(display, x0 + column + char_col, y0 + row + char_row + char_byte * 8, point_color);
                    }
                }
            }
            column += char_width + (h_space ? 1 : 0);
        }
        ++index;
    }
}

void hugo_graphics_print_text_8x8(display_adapter_t *display, int x0, int y0, int width, int height, char *text, bool color)
{
    _print_text(display, x0, y0, width, height, text, color, (uint8_t*)font8x8, 8, 8, false);
}

void hugo_graphics_print_text_8x16(display_adapter_t *display,int x0, int y0, int width, int height, char *text, bool color)
{
    _print_text(display, x0, y0, width, height, text, color, (uint8_t*)font8x16, 8, 16, true);
}

void hugo_graphics_draw_elipse(display_adapter_t *display, int x, int y, int r1, int r2, bool color)
{
    for (int pos = 0; pos < r1; ++pos)
    {
        double normalized = (double)pos / (double)r1;
        int val = (int)(sqrt(1.0 - normalized * normalized) * (double)r2);
        display->set_point(display, x + pos, y - val, color);
        display->set_point(display, x - pos, y - val, color);
        display->set_point(display, x + pos, y + val, color);
        display->set_point(display, x - pos, y + val, color);
    }

    for (int pos = 0; pos < r2; ++pos)
    {
        double normalized = (double)pos / (double)r2;
        int val = (int)(sqrt(1.0 - normalized * normalized) * (double)r1);
        display->set_point(display, x + val, y - pos, color);
        display->set_point(display, x - val, y - pos, color);
        display->set_point(display, x + val, y + pos, color);
        display->set_point(display, x - val, y + pos, color);
    }
}

//static void _draw_line(int x1, int y1, int x2, int y2)
//{
    /*for (int x = x1; x < x2 ; +x)
    {
        __draw_point(x, x * y1 +  , false);

    }*/
//}
