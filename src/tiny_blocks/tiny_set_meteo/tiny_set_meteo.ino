/*
BOARD: hugo_adapter-i2c-pull-ups driving power-block, meteo-block and ble-block
Chip: ATtiny412 or ATtiny1614
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <stdint.h>
#include <Arduino.h>
#include <hugo_defines.h>
#include <tiny_main_base.h>
#include <tiny_main_display.h>
#include <tiny_main_ble.h>
#include <tiny_main_power.h>

#define MESH_MAIN_NODE_ID 0x03

void setup()
{

  tiny_main_base_init();

  tiny_main_display_init();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  tiny_main_power_init();

  //watchdog.enable(Watchdog::TIMEOUT_120MS);
  //_PROTECTED_WRITE(WDT.CTRLA,0);
}


void print_decimal_division(int32_t number, uint16_t divisor){
  bool negative = number < 0;
  int32_t abs = negative ? -number : number;
  if (negative){
    display.print("-");
  }
  uint16_t abs_int = abs / divisor;
  display.print(abs_int);
  display.print('.');

  display.print(abs % divisor);
}

void loop()
{
  //tiny_main_base_set_build_in_led(true);
  tiny_main_ble_mesh_message_size = 3;
  memcpy(tiny_main_ble_mesh_message, ":-D", tiny_main_ble_mesh_message_size);
  tiny_main_ble_send_mesh_data(MESH_MAIN_NODE_ID);

  display.clearDisplay();
  display.setCursor(0,0);

  charging_state_t charging_state = tiny_main_power_get_charging_state();
  display.print(charging_state.is_usb_connected);
  display.println(charging_state.is_charging);

  int32_t voltage_mV = tiny_main_power_get_bat_voltage_mV();
  print_decimal_division(voltage_mV, 1000);
  display.println(" V");

  int32_t current_uA = tiny_main_power_get_bat_current_uA();
  print_decimal_division(current_uA, 1000);
  display.println(" mA");

  display.display();

  delay(100);

  //wdt_disable();  // disable watchdog when running
  //watchdog.reset();
}
