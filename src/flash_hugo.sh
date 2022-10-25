#!/bin/bash
. ../../esp-idf/export.sh

cd ../micropython/ports/esp32/
rm -rf build-HUGO/frozen_mpy
rm -rf build-HUGO/frozen_content.c

idf.py -D MICROPY_BOARD=HUGO -B build-HUGO erase_flash flash -p /dev/ttyUSB0
