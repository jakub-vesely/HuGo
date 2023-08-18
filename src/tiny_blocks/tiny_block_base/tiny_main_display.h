#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "tiny_main_base.h"
#include "hugo_defines.h"

#define OLED_RESET     -1
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 48)
#   error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void tiny_main_display_init(){
    uint8_t ssd_address = tiny_main_base_get_ext_module_address(I2C_BLOCK_TYPE_ID_DISPLAY);
    delay(10);//it is necessary to wait a while to the dispay is started
    display.begin(SSD1306_SWITCHCAPVCC, ssd_address, false);
    display.clearDisplay();
}
