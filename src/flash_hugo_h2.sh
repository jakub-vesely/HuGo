#!/bin/bash
. ../../esp-idf/export.sh

cd ../micropython/ports/esp32/
rm -rf build-HUGO/frozen_mpy
rm -rf build-HUGO/frozen_content.c

port=/dev/ttyUSB0
if [ $# -gt 0 ]; then
  port=$1
fi
#idf.py -D MICROPY_BOARD=HUGO -B build-HUGO erase-flash flash -p $port
#idf.py -D MICROPY_BOARD=HUGO -B build-HUGO erase-flash flash monitor -p $port

idf.py -D MICROPY_BOARD=HUGO_H2 -B build-HUGO_H2 erase-flash flash monitor -p /dev/ttyACM0
#idf.py -D MICROPY_BOARD=HUGO_H2 -B build-HUGO_H2 monitor -p /dev/ttyACM0
#idf.py -D MICROPY_BOARD=HUGO_H2 -B build-HUGO_H2 build
#idf.py monitor -p /dev/ttyACM0
