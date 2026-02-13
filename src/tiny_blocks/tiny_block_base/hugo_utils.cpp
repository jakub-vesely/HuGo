#include "hugo_utils.h"
#include<avr/interrupt.h>

static bool s_sleep_initialized = false;
static RTC_PERIOD_t s_rtc_period=RTC_PERIOD_OFF_gc;
static bool s_adc_enabled = false;

void enable_adc(){
  ADC0.CTRLA |= ADC_ENABLE_bm;
  s_adc_enabled = true;
}

ISR(RTC_PIT_vect){
  RTC.PITINTFLAGS = RTC_PI_bm;          /* Clear interrupt flag by writing '1' (required) */
}

void RTC_init(RTC_PERIOD_t period) //1 sec
{
  if (period == s_rtc_period){
    return; //already set
  }

  /* Initialize RTC: */
  while (RTC_STATUS & RTC_PERBUSY_bm){
    ;                                   /* Wait for all register to be synchronized */
  }
  if (s_rtc_period == RTC_PERIOD_OFF_gc){
    RTC.CLKSEL = RTC_CLKSEL_INT32K_gc;    /* 32.768kHz Internal Ultra-Low-Power Oscillator (OSCULP32K) */
    RTC.PITINTCTRL = RTC_PI_bm;           /* PIT Interrupt: enabled */
  }

  RTC.PITCTRLA = period /* RTC Clock Cycles 16384, resulting in 32.768kHz/32768 = 1Hz */
  | RTC_PITEN_bm;                       /* Enable PIT counter: enabled */

  RTC.PITINTFLAGS = RTC_PI_bm;

  s_rtc_period = period;
}

static void _sleep_init(RTC_PERIOD_t period){
  RTC_init(period);
  if (s_sleep_initialized){
    return;
  }

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  s_sleep_initialized = true;
}

void sleep_for_s(uint16_t timeout_s){
  _sleep_init(RTC_PERIOD_CYC32768_gc);
  for (uint16_t count = 0; count < timeout_s; count++){
    if (s_adc_enabled){
      ADC0.CTRLA &= ~ADC_ENABLE_bm;// Disable ADC - it increase power consumtion
    }
    sleep_cpu();
    if (s_adc_enabled){
      ADC0.CTRLA |= ADC_ENABLE_bm;  // Re-enable after wake if needed
    }
  }
}

static void _sleep_for(RTC_PERIOD_t period){
  _sleep_init(period);
  if (s_adc_enabled){
    ADC0.CTRLA &= ~ADC_ENABLE_bm;// Disable ADC - it increase power consumtion
  }
  sleep_cpu();
  if (s_adc_enabled){
    ADC0.CTRLA |= ADC_ENABLE_bm;  // Re-enable after wake if needed
  }
}

void sleep_for_500ms(){
  _sleep_for(RTC_PERIOD_CYC16384_gc);
}


void sleep_for_250ms(){
  _sleep_for(RTC_PERIOD_CYC8192_gc);
}

void sleep_for_125ms(){
  _sleep_for(RTC_PERIOD_CYC4096_gc);
}
