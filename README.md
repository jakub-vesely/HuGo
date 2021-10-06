![alt text](https://github.com/jakub-vesely/HuGo/blob/master/misc/hugo-logo-b.png "logo")

Main goal of this project is to build a robotic car for my children. Although, it is designed as a common modular system with ambition to be used as a generic educative tool/toy.

![alt text](https://github.com/jakub-vesely/HuGo/blob/master/pcb/HuGo.png "Visualization")

Current version is based on ESP32, and Micropython. Extensive blocks (tyny-blocks) are based on AtTiny 412/416 microcontrollers running Arduino code.

project use forked Micropython https://github.com/jakub-vesely/micropython. HuGo board specific code is placed in the micropython/ports/esp32/boards/HUGO/ directory
and can be flashed by command: idf.py -D MICROPY_BOARD=HUGO -B build-HUGO flash monitor -p <COM PORT>

Python code can be build to the firmware by placing to  /ports/esp32/boards/HUGO/modules
or added by tools/ble_cient.py (code is uplodated wireless via BLE)

## Sources
* Overview of ESP32 features is available here: https://www.espressif.com/en/products/socs/esp32/overview
* esp-idf is linked externaly as recommended. Getting started guide is available here: https://docs.espressif.com/projects/esp-idf/en/stable/get-started/
* Arduino official web page https://www.arduino.cc/
* Arduino suport for AtTiny series 0 and 1 https://github.com/SpenceKonde/megaTinyCore


## Deprecated
* originally was the HuGo project based of LUA but this interpreter is not supported any more
* esp-idf project with LUA support has been moved to src_depricated directory
* LUA 3.5.4 is part of the repository. Original source code is available here: https://www.lua.org/download.html
