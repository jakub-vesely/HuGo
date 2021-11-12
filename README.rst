.. image:: https://github.com/jakub-vesely/HuGo/blob/master/misc/hugo-logo-b.png

Main goal of this project is to build a robotic car for my children. Although, it is designed as a common modular system with ambition to be used as a generic educative tool/toy.

.. image:: https://github.com/jakub-vesely/HuGo/blob/master/pcb/HuGo.png

Current version of the Hugo project is based on ESP32, and Micropython. Extensive blocks (tyny-blocks) are based on AtTiny 412/416 micro-controllers running Arduino code.

project uses forked Micropython https://github.com/jakub-vesely/micropython. HuGo board specific code is located in the micropython/ports/esp32/boards/HUGO/ directory
and can be flashed by command:
"``idf.py -D MICROPY_BOARD=HUGO -B build-HUGO flash monitor -p <COM PORT>``"

-----
Usage
-----

Hugo specific scripts are stored in *micropython/ports/esp32/boards/HUGO/modules*. These files are already prebuilt to the firmware.
User defined behavior should be inserted to *tools/upload/events.py*. Hugo project is optimized for "events driven programming" hance all user defined actions should be added
via planned by one of these methods:

* **plan** (function, \*args, \**kwargs) for performance without expected delay
* **postpone** (delay_s, function, \*args, \**kwargs) for performance an action after a defined delay
* **repeat** (delay_s, function, \*args, \**kwargs) for continuous performed action with a defined delay

**Note**: These planner methods expects action method name defined as parameter **without brackets** followed by its parameters.

Planned action can be removed by the **kill** (handle) method. Appropriate handle is returned by method creating the event.
Programmed behavior can be uploaded to Hugo main block by launching script "``python hugo_terminal.py -f -m``" from the *tools* sub-directory.

Uploading will be performed wireless via BLE.

-----------
Development
-----------

Scripts already placed to firmware can be replaced by uploading files placed in *tools/develop* directory.
These files can be changed and uploaded to the Hugo main block by launching "``python hugo_terminal.py -f -m``" from the *tools* sub-directory.

For integration changes to firmware the original files in *micropython/ports/esp32/boards/HUGO/modules* have to be replaced by updated once.
This action can be performed by launching "``python devel_to_firmware.py``".

To build firmware esp-idf version 4 must be installed.
The build and upload to Hugo main block can be performed by launching:

"``idf.py -D MICROPY_BOARD=HUGO -B build-HUGO erase_flash flash monitor -p <COM PORT>``"
from the *micropython/ports/esp32/* directory.

The parameter *erase_flash* is recommended to remove previously uploaded scripts.

-------
Sources
-------

* Micropython homepage: https://micropython.org/
* Overview of ESP32 features is available here: https://www.espressif.com/en/products/socs/esp32/overview
* esp-idf is linked externally as recommended. Getting started guide is available here: https://docs.espressif.com/projects/esp-idf/en/stable/get-started/
* Arduino official web page https://www.arduino.cc/
* Arduino support for AtTiny series 0 and 1 https://github.com/SpenceKonde/megaTinyCore

----------
Deprecated
----------

* originally was the HuGo project based of LUA but this interpreter is not supported any more
* esp-idf project with LUA support has been moved to *src_deprecated* directory
* LUA 3.5.4 is part of the repository. Original source code is available here: https://www.lua.org/download.html
