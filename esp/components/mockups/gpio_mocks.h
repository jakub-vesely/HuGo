#pragma once

#include <fff.h>
#include <stdint.h>
#include <stdbool.h>

DECLARE_FAKE_VOID_FUNC(fff_gpio_set_pin_for_out, uint8_t);
DECLARE_FAKE_VOID_FUNC(fff_gpio_set_pin_logic_value, uint8_t, bool);
DECLARE_FAKE_VOID_FUNC(fff_gpio_set_pin_for_pwm, int, uint32_t, float);
DECLARE_FAKE_VOID_FUNC(fff_gpio_set_pwm_frequency, int);
DECLARE_FAKE_VOID_FUNC(fff_gpio_set_pwm_duty, float);

#define GPIO_RESET_FAKES() do { \
    RESET_FAKE(fff_gpio_set_pin_for_out); \
    RESET_FAKE(fff_gpio_set_pin_logic_value); \
    RESET_FAKE(fff_gpio_set_pin_for_pwm); \
    RESET_FAKE(fff_gpio_set_pwm_frequency); \
    RESET_FAKE(fff_gpio_set_pwm_duty); \
} while (0)
