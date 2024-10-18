/*
BOARD: hugo_adapter > v6 without i2c-pull-ups
Chip: ATtiny414
Clock Speed: 1 - 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <hugo_defines.h>
#include <tiny_main_base.h>
#include <hugo_gpio.h>

void setup() {
    tiny_main_base_init();
}

void loop() {
    tiny_main_base_set_build_in_led_a(1);
    tiny_main_base_set_build_in_led_b(0);
    delay(500);
    tiny_main_base_set_build_in_led_a(0);
    tiny_main_base_set_build_in_led_b(1);
    delay(500);
    tiny_main_base_set_build_in_led_a(1);
    tiny_main_base_set_build_in_led_b(1);
    delay(500);
    tiny_main_base_set_build_in_led_a(0);
    tiny_main_base_set_build_in_led_b(0);
    delay(500);
}
