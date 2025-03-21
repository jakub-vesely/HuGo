/*
BOARD: hugo_adapter + hugo_button
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>
#include <hugo_defines.h>
#include <hugo_gpio.h>

#define BUTTON_PIN HUGO_PIN_D1
#define I2C_COMMAND_BUTTON_IS_PRESSED 0x01

static bool s_state = false;


void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_BUTTON_IS_PRESSED:
      s_buffer.data[0] = s_state;
      s_buffer.size = 1;
      break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_BUTTON;
  s_buffer.data[1] = HUGO_PCB_VERSION;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(BUTTON_PIN, INPUT);
    HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_BUTTON, NULL);
}

// the loop function runs over and over again forever
void loop()
{
    s_state = digitalRead(BUTTON_PIN);
    //TODO: an interupt can be used instead of pooling it could decrease power consumption
    delay(10); // 10 ms should be enough for button
}
