/*
BOARD: hugo_adapter + hugo_ble jdy-25
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <stdint.h>

#define AUTO_DEEP_SLEEP_DISABLED //to be disabled deep sleep in hugo_tiny_wire.h

#include <avr/sleep.h>
#include "tiny_block_ble.h"


void setup()
{
  init_ble();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void loop()
{
  ble_main();

  if (s_flags.sleep_me && s_flags.expected_resp_count == 0){
    sleep_cpu();
  }
}
