#!/usr/bin/env python
import sys
import png

reader = png.Reader(filename="./hugo-8x16.png")
data = reader.read()
in_rows = data[2]
data = list()
for in_row in in_rows:
    data_row = list()
    element_nr = 0
    for byte in in_row:
        if element_nr == 0: #not importan which from R,G,B (its 0 or 255 for all of them)
            data_row.append(byte)
        element_nr += 1
        element_nr %= 4 #R+G+B+alpha
    data.append(data_row)

char_count = 256
chars_per_row = 16
char_width = 8
char_height = 16
h_space_wisth_width = 1
v_space_width = 1
chars = list()
for char_nr in range(char_count):
    chars.append(list())
    start_column = (char_nr % chars_per_row) * (char_width + h_space_wisth_width)
    start_row = (int)(char_nr / chars_per_row) * (char_height + v_space_width)
    for x in range(char_width):
        column_sum = 0
        for y in range(char_height, 0, -1):
            column_sum = (column_sum << 1) + (1 if data[start_row + y][start_column + x] else 0)
        chars[char_nr].append(column_sum)

first = True
order = 0;
with open ("./hugo-8x16.h", "w")  as h_file:
    for char in chars:
        if not first:
            #h_file.write("}, // %s %s\n    {" % (hex(order), (chr(ord(" ") - 32 + order) if order > 32 and order < 128 else "")))
            h_file.write("}, // %s\n    {" % (hex(order)))
            order += 1;
        else:
            first = False
            h_file.write("#pragma once\n\n#include <stdint.h>\nuint16_t font8x16_basic_tr[256][8] = {\n    {")

        for item in char:
            h_file.write("0x{:04x}, ".format(item))
    h_file.write("}\n};\n")
