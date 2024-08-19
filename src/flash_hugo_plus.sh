#!/bin/bash
. ../../esp-idf/export.sh

cd ../micropython/ports/esp32/
rm -rf build-HUGO/frozen_mpy
rm -rf build-HUGO/frozen_content.c

port=/dev/ttyUSB0
if [ $# -gt 0 ]; then
  port=$1
fi
idf.py -D MICROPY_BOARD=HUGO-PLUS -B build-HUGO-PLUS erase_flash flash -p $port
