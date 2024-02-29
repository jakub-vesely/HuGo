/*
BOARD: hugo_adapter + hugo_rj12
Chip: ATtiny412
Clock Speed: 1MHz
Options: milis-disabled (probably not required)
Programmer: jtag2updi (megaTinyCore)
*/

void HugoTinyWireProcessCommand(uint8_t command, uint8_t payload_size);
void HugoTinyWireFillModuleVersion();

#include <avr/sleep.h>
#include <hugo_tiny_wire.h>
#include <util/atomic.h>

#define PCB_VERSION 1
#define ADJUSTMENT_VERSION 1

#define RJ_PIN4 PIN_PA6
#define RJ_PIN5 PIN_PA3

uint16_t oscillation_period_ms = 0;

volatile uint32_t timestamps[2] = {0, 0};
volatile uint16_t pin_counter;
volatile uint32_t rtc_counter=0;
volatile int8_t rtc_correction=0;

void pin_counter_isr(){
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    if (rtc_counter > timestamps[1] && rtc_counter - timestamps[1] > oscillation_period_ms){
      pin_counter++;

      timestamps[0] = timestamps[1];
      timestamps[1] = rtc_counter;
    }
  }
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
    case I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_RISI:
      pinMode(RJ_PIN4, INPUT_PULLUP);
      noInterrupts();
      attachInterrupt(digitalPinToInterrupt(RJ_PIN4), pin_counter_isr, RISING);
      interrupts();
    break;

    case I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_FALL:
      pinMode(RJ_PIN4, INPUT_PULLUP);
      noInterrupts();
      attachInterrupt(digitalPinToInterrupt(RJ_PIN4), pin_counter_isr, FALLING);
      interrupts();
    break;

    case I2C_COMMAND_RJ12_PIN4_SET_AS_COUNTER_CHNG:
      pinMode(RJ_PIN4, INPUT_PULLUP);
      noInterrupts();
      attachInterrupt(digitalPinToInterrupt(RJ_PIN4), pin_counter_isr, CHANGE);
      interrupts();
    break;

    case I2C_COMMAND_RJ12_PIN4_SET_OSCIL_PERIOD_MS:
      oscillation_period_ms = HugoTinyWireRead();
      oscillation_period_ms += HugoTinyWireRead() << 8;
    break;

    case I2C_COMMAND_RJ12_PIN4_GET_COUNT_AND_RESET:
      ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        s_buffer.data[0] = (pin_counter & 0xff);
        s_buffer.data[1] = (pin_counter >> 8);
        s_buffer.size = 2;
        pin_counter = 0;
      }
    break;
    case I2C_COMMAND_RJ12_PIN4_GET_LAST_TIMESTAMPS:{
      noInterrupts(); //ATOMIC_BLOCK can't be used here due to initialization of variables
      uint32_t diff1 = timestamps[1] - timestamps[0];
      uint32_t diff2 = rtc_counter - timestamps[1] ;
      interrupts();

      s_buffer.data[0] = diff1 & 0xff;
      s_buffer.data[1] = (diff1 >> 8) & 0xff;
      s_buffer.data[2] = (diff1 >> 16) & 0xff;
      s_buffer.data[3] = (diff1 >> 24) & 0xff;

      s_buffer.data[4] = diff2 & 0xff;
      s_buffer.data[5] = (diff2 >> 8) & 0xff;
      s_buffer.data[6] = (diff2 >> 16) & 0xff;
      s_buffer.data[7] = (diff2 >> 24) & 0xff;

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

void RTC_init(void)
{
  /* Initialize RTC: */
  while (RTC.STATUS > 0)
  {
    ;                                   /* Wait for all register to be synchronized */
  }
  RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    /* 32.768kHz Internal Ultra-Low-Power Oscillator (OSCULP32K) */

  RTC.PITINTCTRL = RTC_PI_bm;           /* PIT Interrupt: enabled */

  RTC.PITCTRLA = RTC_PERIOD_CYC512_gc | /* in 512/32.768kHz = 0.015625 ms */
  RTC_PITEN_bm;                        /* Enable PIT counter: enabled */
}

ISR(RTC_PIT_vect)
{
  RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */
  rtc_counter += 15; //one interrupt each 0.015625 sec
  rtc_correction += 1;
  if (rtc_correction == 8){ //0.625 * 8 = 5
    rtc_correction = 0;
    rtc_counter += 5;
  }
}

void setup()
{
  RTC_init();

  pin_counter = 0;
  pinMode(RJ_PIN4, INPUT);
  pinMode(RJ_PIN5, INPUT);

  timestamps[0] = 0;
  timestamps[1] = 0;
  HugoTinyWireInitialize(I2C_BLOCK_TYPE_ID_RJ12, NULL);
}

void loop()
{
  sleep_cpu();
}
