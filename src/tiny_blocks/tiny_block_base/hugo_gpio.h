#pragma once

#ifndef HUGO_PCB_VERSION
#   error HUGO_PCB_VERSION is not defined
#else
#   if HUGO_PCB_VERSION < 7
#       define HUGO_PIN_D1 PIN_PA7
#       define HUGO_PIN_D2 PIN_PA6
#       define HUGO_PIN_D3 PIN_PA3
//#       define HUGO_PIN_D4
//#       define HUGO_PIN_D5

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
//#           define HUGO_PIN_LED_A
//#           define HUGO_PIN_SHIELD_POWER

#           define IS_I2C_ALTERNATIVE false
        else
#           define HUGO_PIN_LED_A PIN_PA4
#           define HUGO_PIN_SHIELD_POWER

            define IS_I2C_ALTERNATIVE true
#       endif
#       define IS_UART_ALTERNATIVE false
#   else
#       define HUGO_PIN_D1 PIN_PA1
#       define HUGO_PIN_D2 PIN_PA2
#       define HUGO_PIN_D3 PIN_PA3
#       define HUGO_PIN_D4 PIN_PA4
#       define HUGO_PIN_D5 PIN_PA5

#       define HUGO_PIN_RXD PIN_PA2 //alternative RXD
#       define HUGO_PIN_TXD PIN_PA1 //alternative TXD

#       define HUGO_PIN_SDA PIN_PB1
#       define HUGO_PIN_SCL PIN_PB0

#       define HUGO_PIN_MOSI  PIN_PA1
#       define HUGO_PIN_MISO PIN_PA12
#       define HUGO_PIN_SCK PIN_PA3
#       define HUGO_PIN_SS PIN_PA4
#       define HUGO_PIN_VREF PIN_PA5

#       define HUGO_PIN_LED_A PIN_PA6
#       if HUGO_PCB_VERSION == 7
#           define HUGO_PIN_LED_B PIN_PB3
#       else
#           define HUGO_PIN_LED_B PA7
#       endif
#       define HUGO_PIN_SHIELD_POWER PIN_PB2

#       define IS_I2C_ALTERNATIVE false
#       define IS_UART_ALTERNATIVE true
#   endif
#endif

inline void hugo_gpio_initialize(){
    //set all pins as pulled-up inputs to reduce power consumption
    pinMode(PIN_PA1, INPUT_PULLUP);
    pinMode(PIN_PA2, INPUT_PULLUP);
    pinMode(PIN_PA3, INPUT_PULLUP);
    pinMode(PIN_PA6, INPUT_PULLUP);
    pinMode(PIN_PA7, INPUT_PULLUP);

#if !defined(__AVR_ATtiny412__)
    pinMode(PIN_PA4, INPUT_PULLUP);
    pinMode(PIN_PA5, INPUT_PULLUP);
    pinMode(PIN_PB0, INPUT_PULLUP);
    pinMode(PIN_PB1, INPUT_PULLUP);
    pinMode(PIN_PB2, INPUT_PULLUP);
    pinMode(PIN_PB3, INPUT_PULLUP);

    pinMode(HUGO_PIN_SHIELD_POWER, OUTPUT);
    digitalWrite(HUGO_PIN_SHIELD_POWER, 1);

    if (IS_I2C_ALTERNATIVE){
        PORTMUX.CTRLB |= PORTMUX_TWI0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
    }

     if (IS_UART_ALTERNATIVE){
        PORTMUX.CTRLB |= PORTMUX_USART0_ALTERNATE_gc; //_ATtiny414 uses alternative ports for i2c because of conflict with the POWER_SAVE_PIN
    }
#endif
}
