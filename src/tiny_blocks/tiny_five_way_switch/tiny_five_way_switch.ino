/*
BOARD: hugo_adapter + hugo_button
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>

#define PCB_VERSION 2
#define ADJUSTMENT_VERSION 0

#define BUTTON_PIN PIN_PA3
#define I2C_COMMAND_BUTTON_ID 1
#define I2C_COMMAND_BUTTON_VALUE 2

#define NO_BUTTON 0
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define UP_BUTTON 3
#define DOWN_BUTTON 4
#define PRESS_BUTTON 5

static uint8_t s_value = 0;
static uint8_t s_state = NO_BUTTON;


void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_BUTTON_VALUE:
      s_buffer.data[0] = s_value;
      s_buffer.size = 1;
      break;
    case I2C_COMMAND_BUTTON_ID:
      s_buffer.data[0] = s_state;
      s_buffer.size = 1;
      break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_BUTTON;
  s_buffer.data[1] = PCB_VERSION;
  s_buffer.data[2] = ADJUSTMENT_VERSION;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup()
{
    pinMode(BUTTON_PIN, INPUT);
    HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_BUTTON, NULL);
}

// the loop function runs over and over again forever
void loop()
{
    s_value = analogRead(BUTTON_PIN);
    if (s_value < 10){
      s_state = UP_BUTTON;
    }
    else if (s_value < 56){
      s_state = LEFT_BUTTON;
    }
    else if (s_value < 100){
      s_state = RIGHT_BUTTON;
    }
    else if (s_value < 200){
      s_state = PRESS_BUTTON;
    }
    else if (s_value < 240){
      s_state = DOWN_BUTTON;
    }
    else{
      s_state = NO_BUTTON;
    }

    delay(10); // 10 ms should be enough for button
}
