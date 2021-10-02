/*
BOARD: hugo_adapter + hugo_rgb
Chip: ATtiny412
Clock Speed: 10MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <hugo_tiny_wire.h>
#include <hugo_tiny_block_defines.h>
#include "tiny_block_rgb_led_arduino.h"

//#include <Time.h>
#define LED_R PIN_PA6
#define LED_G PIN_PA7
#define LED_B PIN_PA3
#define PWM_STEP 5

#define R_COMP 50 //max is 100
#define G_COMP 100
#define B_COMP 75

const uint8_t black[3] = {0, 0 ,0};
const uint8_t white[3] = {255, 255 ,255};

const uint8_t red[3] = {255, 0 ,0};
const uint8_t green[3] = {0, 255, 0};
const uint8_t blue[3] = {0, 0 ,255};

const uint8_t yellow[3] = {255, 225,0};
const uint8_t purple[3] = {255, 0 ,255};
const uint8_t cyan[3] = {0, 255, 255};

const uint8_t orange[3] = {255, 127,0};
const uint8_t greenyellow[3] = {127, 255,0};
const uint8_t skyblue[3] = {0, 127,255};
const uint8_t aquamarine[3] = {0, 255, 127};

const uint8_t magenta[3] = {255, 0, 127};
const uint8_t violet[3] = {127, 0, 255};
const uint8_t* ext_addresses = { 0x00 };

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
    case I2C_COMMAND_RGB_SET_ON:
      memcpy(rgb, white, 3);
      break;
    case I2C_COMMAND_RGB_SET_OFF:
      memcpy(rgb, black, 3);
    break;

    case I2C_COMMAND_RGB_SET_COLOR:
      if (payload_size > 0){
        uint8_t color = HugoTinyWireRead();
        switch (color){
          case RGB_LED_CLOLOR_BLACK:
            memcpy(rgb, black, 3);
            break;
          case RGB_LED_CLOLOR_WHITE:
            memcpy(rgb, white, 3);
            break;
          case RGB_LED_CLOLOR_RED:
            memcpy(rgb, red, 3);
            break;
          case RGB_LED_CLOLOR_GREEN:
            memcpy(rgb, green, 3);
            break;
          case RGB_LED_CLOLOR_BLUE:
            memcpy(rgb, blue, 3);
            break;
          case RGB_LED_CLOLOR_YELLOW:
            memcpy(rgb, yellow, 3);
            break;
          case RGB_LED_CLOLOR_PURPLE:
            memcpy(rgb, purple, 3);
            break;
          case RGB_LED_CLOLOR_CYAN:
            memcpy(rgb, cyan, 3);
            break;
          case RGB_LED_CLOLOR_ORANGE:
            memcpy(rgb, orange, 3);
            break;
          case RGB_LED_CLOLOR_GREENYELLOW:
            memcpy(rgb, greenyellow, 3);
            break;
          case RGB_LED_CLOLOR_SKYBLUE:
            memcpy(rgb, skyblue, 3);
            break;
          case RGB_LED_CLOLOR_AQUAMARINE:
            memcpy(rgb, aquamarine, 3);
            break;
          case RGB_LED_CLOLOR_MAGENTA:
            memcpy(rgb, magenta, 3);
            break;
          case RGB_LED_CLOLOR_VIOLET:
            memcpy(rgb, violet, 3);
            break;
        }
      }
      break;
  }
}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_RGB, ext_addresses);
}

// the loop function runs over and over again forever
void loop()
{
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
