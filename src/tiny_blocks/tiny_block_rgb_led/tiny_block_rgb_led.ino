/*
BOARD: hugo_adapter + hugo_rgb
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>
#include <hugo_gpio.h>

#define PCB_VERSION 1
#define ADJUSTMENT_VERSION 1
//#include <Time.h>
#define LED_R HUGO_PIN_D2
#define LED_G HUGO_PIN_D1
#define LED_B HUGO_PIN_D3
#define PWM_STEP 5

#define R_COMP 50 //max is 100
#define G_COMP 100
#define B_COMP 75

#define I2C_COMMAND_RGB_SET_RGB 1 // followed by 3 bytes (RGB)

const uint8_t black[3] = {0, 0 ,0};
uint8_t rgb[3] = {255, 255, 255}; //default is white/ON

int counter = 0;
int start_countdown = 20000;

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_RGB_SET_RGB:
      if (payload_size > 2){
        rgb[0] = HugoTinyWireRead();
        rgb[1] = HugoTinyWireRead();
        rgb[2] = HugoTinyWireRead();
      }
      break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_RGB;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){

}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_RGB, NULL);
}

void loop(){
    //It is not possible to use analog write because pin0 is does not support PWM
    digitalWrite(LED_R, counter < rgb[0] * R_COMP / 100 ? HIGH : LOW);
    digitalWrite(LED_G, counter < rgb[1] * G_COMP / 100 ? HIGH : LOW);
    digitalWrite(LED_B, counter < rgb[2] * B_COMP / 100 ? HIGH : LOW);

    counter++;
    if (counter > 254)
        counter = 0;
    delayMicroseconds(PWM_STEP);

    if (start_countdown > 0){
      start_countdown -= PWM_STEP;
      if (start_countdown == 0){
        memcpy(rgb, black, 3);
      }
    }
}
