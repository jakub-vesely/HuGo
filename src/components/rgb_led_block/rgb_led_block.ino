/*
Blink
Turns an LED on for one second, then off for one second, repeatedly.

BOARD: ATtiny1614/1604/814/804/414/404/214/204
Chip: ATtiny1614
Clock Speed: 20MHz
Programmer: jtag2updi (megaTinyCore)
*/

#include <Wire.h>
//#include <Time.h>
#define LED_R PIN_PA6
#define LED_G PIN_PA7
#define LED_B PIN_PA3
#define PWM_STEP 10

#define R_COMP 50 //max is 100
#define G_COMP 100
#define B_COMP 75

#define I2C_COMMAND_SET_RGB 64 // followed by 3 bytes (RGB)
#define I2C_COMMAND_SET_ON 66
#define I2C_COMMAND_SET_OFF 67

#define I2C_COMMAND_SET_BLACK 100
#define I2C_COMMAND_SET_WHITE 101
#define I2C_COMMAND_SET_RED 102
#define I2C_COMMAND_SET_GREEN 103
#define I2C_COMMAND_SET_BLUE 104

#define I2C_COMMAND_SET_YELLOW 105
#define I2C_COMMAND_SET_PURPLE 106
#define I2C_COMMAND_SET_CYAN 107

#define I2C_COMMAND_SET_ORANGE 108
#define I2C_COMMAND_SET_GREENYELLOW 109
#define I2C_COMMAND_SET_SKYBLUE 110
#define I2C_COMMAND_SET_AQUAMARINE 111
#define I2C_COMMAND_SET_MAGENTA 112
#define I2C_COMMAND_SET_VIOLET 113

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


uint8_t rgb[3] = {255, 255, 255}; //default is white/ON

int counter = 0;
int start_countdown = 20000;


bool r_to_b = true;
bool b_to_g = false;
bool g_to_r = false;

// the setup function runs once when you press reset or power the board
void i2c_receive_data(int count) {
    if (count > 0){
      int command = Wire.read();
      switch (command) {
        case I2C_COMMAND_SET_RGB:
          rgb[0] = Wire.read();
          rgb[1] = Wire.read();
          rgb[2] = Wire.read();
        break;
        case I2C_COMMAND_SET_ON:
        case I2C_COMMAND_SET_WHITE:
          memcpy(rgb, white, 3);
        break;
        case I2C_COMMAND_SET_OFF:
        case I2C_COMMAND_SET_BLACK:
          memcpy(rgb, black, 3);
        break;
        case I2C_COMMAND_SET_RED:
          memcpy(rgb, red, 3);
        break;
        case I2C_COMMAND_SET_GREEN:
          memcpy(rgb, green, 3);
        break;
        case I2C_COMMAND_SET_BLUE:
          memcpy(rgb, blue, 3);
        break;
        case I2C_COMMAND_SET_YELLOW:
          memcpy(rgb, yellow, 3);
        break;
        case I2C_COMMAND_SET_PURPLE:
          memcpy(rgb, purple, 3);
        break;
        case I2C_COMMAND_SET_CYAN:
          memcpy(rgb, cyan, 3);
        break;
        case I2C_COMMAND_SET_ORANGE:
          memcpy(rgb, cyan, 3);
        break;
        case I2C_COMMAND_SET_GREENYELLOW:
          memcpy(rgb, orange, 3);
        break;
        case I2C_COMMAND_SET_SKYBLUE:
          memcpy(rgb, skyblue, 3);
        break;
        case I2C_COMMAND_SET_AQUAMARINE:
          memcpy(rgb, aquamarine, 3);
        break;
        case I2C_COMMAND_SET_MAGENTA:
          memcpy(rgb, magenta, 3);
        break;
        case I2C_COMMAND_SET_VIOLET:
          memcpy(rgb, violet, 3);
        break;
        default:
          memcpy(rgb, red, 3);
        break;
      }
    }
}

void i2c_request_event(){

    //simulated_registers[0] = 0x08;
    /*Wire.write(counter);
    counter++;
    if (counter == 100)
        counter = 0;
    */
}

void setup()
{
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);

    Wire.begin(0x02);
    Wire.onReceive(i2c_receive_data);
    Wire.onRequest(i2c_request_event);
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
