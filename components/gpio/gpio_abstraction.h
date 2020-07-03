#ifdef UNIT_TEST
#   include "gpio_mocks.h"

#   define HUGO_GPIO_SET_PIN_FOR_OUT(pin) fff_gpio_set_pin_for_out(pin)
#   define HUGO_GPIO_SET_PIN_LOGIC_VALUE(pin, value) fff_gpio_set_pin_logic_value(pin, value)
#   define HUGO_GPIO_SET_PIN_FOR_PWM(pin, frequency, duty) fff_gpio_set_pin_for_pwm(pin, frequency, duty)
#   define HUGO_GPIO_SET_PWM_FREQUENCY(frequency) fff_gpio_set_pwm_frequency(frequency)
#   define HUGO_GPIO_SET_PWM_DUTY(duty) fff_gpio_set_pwm_duty(duty)

#else
#   include "gpio.h"

#   define HUGO_GPIO_SET_PIN_FOR_OUT(pin) hugo_gpio_set_pin_for_out(pin)
#   define HUGO_GPIO_SET_PIN_LOGIC_VALUE(pin, value) hugo_gpio_set_pin_logic_value(pin, value)
#   define HUGO_GPIO_SET_PIN_FOR_PWM(pin, frequency, duty) hugo_gpio_set_pin_for_pwm(pin, frequency, duty)
#   define HUGO_GPIO_SET_PWM_FREQUENCY(frequency) hugo_gpio_set_pwm_frequency(frequency)
#   define HUGO_GPIO_SET_PWM_DUTY(duty) hugo_gpio_set_pwm_duty(duty)

#endif
