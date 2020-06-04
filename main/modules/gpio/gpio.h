/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#ifndef _HUGO_GPIO_H_
#define _HUGO_GPIO_H_

#include "external/lua/src/lua.h"
#include <stdbool.h>
#include <stdint.h>

void hugo_gpio_init_module(lua_State* L);

void hugo_gpio_set_pin_for_out(uint8_t pin);
void hugo_gpio_set_pin_logic_value(uint8_t pin, bool value);
void hugo_gpio_set_pin_for_pwm(int pin, uint32_t frequency, float duty);
void hugo_gpio_set_pwm_frequency(int frequency);
void hugo_gpio_set_pwm_duty(float duty);

#endif //_HUGO_GPIO_H_
