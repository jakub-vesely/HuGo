#pragma once

#ifndef HUGO_PCB_VERSION
#   error HUGO_PCB_VERSION is not defined
#else
#   if HUGO_PCB_VERSION < 7
#       define HUGO_PIN_D1 PIN_PA7
#       define HUGO_PIN_D2 PIN_PA6
#       define HUGO_PIN_D3 PIN_PA3

#       if defined(__AVR_ATtiny412__)
//Serial communication for old PCBs is not implemented any more
//#           define HUGO_PIN_RXD PIN_PB2
//#           define HUGO_PIN_TXD PIN_PB3

#       else
//#           define HUGO_PIN_RXD
//#           define HUGO_PIN_TXD
#       endif

#       define HUGO_PIN_SDA PIN_PA1
#       define HUGO_PIN_SCL PIN_PA2
//#       define HUGO_PIN_MOSI
//#       define HUGO_PIN_MISO
//#       define HUGO_PIN_SCK
//#       define HUGO_PIN_SS
//#       define HUGO_PIN_VREF

//#       define HUGO_PIN_LED_B

#       if defined(__AVR_ATtiny412__)
#           define IS_I2C_ALTERNATIVE false
#        else
#           define HUGO_PIN_LED_A PIN_PA4
#           define HUGO_PIN_SHIELD_POWER PIN_PB1

#            define IS_I2C_ALTERNATIVE true
#       endif
#       define IS_UART_ALTERNATIVE false
#   else
#       define HUGO_PIN_D1 PIN_PA1
#       define HUGO_PIN_D2 PIN_PA2
#       define HUGO_PIN_D3 PIN_PA3
#       define HUGO_PIN_D4 PIN_PA4
#       if HUGO_PCB_VERSION == 7
#           define HUGO_PIN_D5 PIN_PA5
#       else
#            define HUGO_PIN_D5 PIN_PB2
#       endif
#       define HUGO_PIN_RXD PIN_PA2 //alternative RXD
#       define HUGO_PIN_TXD PIN_PA1 //alternative TXD

#       define HUGO_PIN_SDA PIN_PB1
#       define HUGO_PIN_SCL PIN_PB0

#       define HUGO_PIN_MOSI  PIN_PA1
#       define HUGO_PIN_MISO PIN_PA12
#       define HUGO_PIN_SCK PIN_PA3
#       define HUGO_PIN_SS PIN_PA4
#       define HUGO_PIN_LED_A PIN_PA6
#       if HUGO_PCB_VERSION == 7
#           define HUGO_PIN_VREF PIN_PA5
#           define HUGO_PIN_LED_B PIN_PB3
#           define HUGO_PIN_SHIELD_POWER PIN_PB2
#       else
#           define HUGO_PIN_LED_B PIN_PA7
#           define HUGO_PIN_SHIELD_POWER PIN_PB3
#       endif
#
#       define IS_I2C_ALTERNATIVE false
#       define IS_UART_ALTERNATIVE true
#   endif
#endif

inline void hugo_gpio_init_serial(){
#if !defined(__AVR_ATtiny412__) && HUGO_PCB_VERSION < 7
  pinMode(HUGO_PIN_D2, INPUT); //to be used PB3 and PB2 instead
  pinMode(HUGO_PIN_D1, INPUT);
#else
  pinMode(HUGO_PIN_D2, INPUT_PULLUP);
  pinMode(HUGO_PIN_D1, OUTPUT);
#endif

    if (IS_UART_ALTERNATIVE){
#if !defined(__AVR_ATtiny824__) && !defined(__AVR_ATtiny1624__) && !defined(__AVR_ATtiny3224__)
        PORTMUX.CTRLB |= PORTMUX_USART0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
#else
        //NOTE: ports swapped in BleShield::init
        //PORTMUX.USARTROUTEA |= PORTMUX_USART1_NONE_gc;
        //PORTMUX.USARTROUTEA |= PORTMUX_USART0_ALT1_gc; //all 24s use alternative UART as sane as 14s. 24s use different name for the same value as 14s
#endif
    }
}

inline void hugo_gpio_initialize(){
    //set all pins as pulled-up inputs to reduce power consumption
    pinMode(PIN_PA1, INPUT_PULLUP);
    pinMode(PIN_PA2, INPUT_PULLUP);
    //pinMode(PIN_PA3, INPUT_PULLUP);
    pinMode(PIN_PA6, INPUT_PULLUP);
    pinMode(PIN_PA7, INPUT_PULLUP);

#if !defined(__AVR_ATtiny412__)
    pinMode(PIN_PA4, INPUT_PULLUP);
    pinMode(PIN_PA5, INPUT_PULLUP);
    pinMode(PIN_PB0, INPUT_PULLUP);
    pinMode(PIN_PB1, INPUT_PULLUP);
    pinMode(PIN_PB2, INPUT_PULLUP);
    pinMode(PIN_PB3, INPUT_PULLUP);

    pinMode(HUGO_PIN_LED_A, OUTPUT);
#   if HUGO_PCB_VERSION >= 7
        pinMode(HUGO_PIN_SHIELD_POWER, OUTPUT);
        digitalWrite(HUGO_PIN_SHIELD_POWER, 1);
        pinMode(HUGO_PIN_LED_B, OUTPUT);
#   endif

    if (IS_I2C_ALTERNATIVE){
#   if !defined(__AVR_ATtiny824__) && !defined(__AVR_ATtiny1624__) && !defined(__AVR_ATtiny3224__)
        PORTMUX.CTRLB |= PORTMUX_TWI0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
#   endif
    }

    hugo_gpio_init_serial(); //just for sure. it was here for previous versions <7
#endif
}
