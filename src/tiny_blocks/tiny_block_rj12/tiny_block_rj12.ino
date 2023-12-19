/*
BOARD: hugo_adapter + hugo_rj12
Chip: ATtiny412
Clock Speed: 1MHz
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <hugo_tiny_wire.h>

#define PCB_VERSION 1
#define ADJUSTMENT_VERSION 1

#define RJ_PIN4 PIN_PA6
#define RJ_PIN5 PIN_PA3

static uint32_t timestamps[2] = {0, 0};
static uint16_t oscillation_period_ms;
static uint16_t counter = 0;
void count1(){
  uint32_t now = millis();
  if (now - timestamps[1] < oscillation_period_ms){
    return; //reduce switch oscillations
  }

  counter += 1;
  timestamps[0] = timestamps[1];
  timestamps[1] = now;
}

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size) {
  switch (command) {
    case I2C_COMMAND_RJ12_PIN4_SET_AS_IN:
      pinMode(RJ_PIN4, INPUT);
    break;
    case I2C_COMMAND_RJ12_PIN5_SET_AS_IN:
      pinMode(RJ_PIN5, INPUT);
    break;
    case I2C_COMMAND_RJ12_PIN4_SET_AS_OUT:
      pinMode(RJ_PIN4, OUTPUT);
    break;
    case I2C_COMMAND_RJ12_PIN5_SET_AS_OUT:
      pinMode(RJ_PIN5, OUTPUT);
    break;
    case I2C_COMMAND_RJ12_PIN4_SET_DIG_VALUE:
      if (payload_size > 0){
        digitalWrite(RJ_PIN4, HugoTinyWireRead());
      }
    break;
    case I2C_COMMAND_RJ12_PIN5_SET_DIG_VALUE:
      if (payload_size > 0){
        digitalWrite(RJ_PIN5, HugoTinyWireRead());
      }
    break;
    case I2C_COMMAND_RJ12_PIN4_GET_DIG_VALUE:
      s_buffer.data[0] = digitalRead(RJ_PIN4);
      s_buffer.size = 1;
    break;
    case I2C_COMMAND_RJ12_PIN5_GET_DIG_VALUE:
      s_buffer.data[0] = digitalRead(RJ_PIN5);
      s_buffer.size = 1;
    break;
    case I2C_COMMAND_RJ12_PIN4_GET_ANA_VALUE: {
      int value = analogRead(RJ_PIN4);
      s_buffer.data[0] = value & 0xff;
      s_buffer.data[1] = value >> 8;
      s_buffer.size = 2;
    }
    break;
    case I2C_COMMAND_RJ12_PIN5_GET_ANA_VALUE: {
      int value = analogRead(RJ_PIN5);
      s_buffer.data[0] = value & 0xff;
      s_buffer.data[1] = value >> 8;
      s_buffer.size = 2;
    }
    break;
    case I2C_COMMAND_RJ12_PIN5_SET_AS_COUNTER_RISI:
      pinMode(RJ_PIN5, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(RJ_PIN5), count1, RISING);
    break;

    case I2C_COMMAND_RJ12_PIN5_SET_AS_COUNTER_FALL:
      pinMode(RJ_PIN5, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(RJ_PIN5), count1, FALLING);
    break;

    case I2C_COMMAND_RJ12_PIN5_SET_AS_COUNTER_CHNG:
      pinMode(RJ_PIN5, INPUT_PULLUP);
      attachInterrupt(digitalPinToInterrupt(RJ_PIN5), count1, CHANGE);
    break;

    case I2C_COMMAND_RJ12_PIN5_SET_OSCIL_PERIOD_MS:
      oscillation_period_ms = HugoTinyWireRead();
      oscillation_period_ms += s_buffer.data[1] << 8;
    break;

    case I2C_COMMAND_RJ12_PIN5_GET_COUNT_AND_RESET:
      noInterrupts();
      s_buffer.data[0] = counter & 0xff;
      s_buffer.data[1] = counter >> 8;
      //counter = 0;
      interrupts();
      s_buffer.size = 2;
    break;
    case I2C_COMMAND_RJ12_PIN5_GET_LAST_TIMESTAMPS:{
      uint32_t now = millis();
      noInterrupts();

      uint32_t diff1 = timestamps[1] - timestamps[0];
      uint32_t diff2 = now - timestamps[1] ;

      s_buffer.data[0] = diff1 & 0xff;
      s_buffer.data[1] = diff1 >> 8 & 0xff;
      s_buffer.data[2] = diff1 >> 16 & 0xff;
      s_buffer.data[3] = diff1 >> 24 & 0xff;

      s_buffer.data[4] = diff2 & 0xff;
      s_buffer.data[5] = diff2 >> 8 & 0xff;
      s_buffer.data[6] = diff2 >> 16 & 0xff;
      s_buffer.data[7] = diff2 >> 24 & 0xff;

      interrupts();

      s_buffer.size = 8;
    }
    break;
  }
}

void HugoTinyWireFillModuleVersion(){
  s_buffer.data[0] = I2C_BLOCK_TYPE_ID_RJ12;
  s_buffer.data[1] = 0;
  s_buffer.data[2] = 0;
  s_buffer.size = 3;
}

void HugoTinyWirePowerSave(uint8_t level){
}

void setup()
{
  pinMode(RJ_PIN4, INPUT);
  pinMode(RJ_PIN5, INPUT);

  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_RJ12, NULL);
}

void loop()
{
}
