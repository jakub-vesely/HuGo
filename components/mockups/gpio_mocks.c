#include "gpio_mocks.h"

DEFINE_FAKE_VOID_FUNC(fff_gpio_set_pin_for_out, uint8_t);
DEFINE_FAKE_VOID_FUNC(fff_gpio_set_pin_logic_value, uint8_t, bool);
DEFINE_FAKE_VOID_FUNC(fff_gpio_set_pin_for_pwm, int, uint32_t, float);
DEFINE_FAKE_VOID_FUNC(fff_gpio_set_pwm_frequency, int);
DEFINE_FAKE_VOID_FUNC(fff_gpio_set_pwm_duty, float);
