#ifdef UNIT_TEST
#   include "test/gpio_mocks.h"

#   define GPIO_PAD_SELECT_GPIO(gpio_num) fff_gpio_pad_select_gpio(gpio_num)
#   define GPIO_SET_DIRECTION(gpio_num, mode) fff_gpio_set_direction(gpio_num, mode)
#   define GPIO_SET_LEVEL(gpio_num, level) fff_gpio_set_level(gpio_num, level)

#   define MCPWM_GPIO_INIT(mcpwm_num, io_signal, gpio_num) fff_mcpwm_gpio_init(mcpwm_num, io_signal, gpio_num)
#   define MCPWM_INIT(mcpwm_num, timer_num, mcpwm_conf) fff_mcpwm_init(mcpwm_num, timer_num, mcpwm_conf)
#   define MCPWM_SET_DUTY(mcpwm_num, timer_num, gen, duty) fff_mcpwm_set_duty(mcpwm_num, timer_num, gen, duty)
#   define MCPWM_SET_FREQUENCY(mcpwm_num, timer_num, frequency) fff_mcpwm_set_frequency(mcpwm_num, timer_num, frequency)


#else
#   include <driver/gpio.h>
#   include <driver/mcpwm.h>

#   define GPIO_PAD_SELECT_GPIO(gpio_num) gpio_pad_select_gpio(gpio_num)
#   define GPIO_SET_DIRECTION(gpio_num, mode) gpio_set_direction(gpio_num, mode)
#   define GPIO_SET_LEVEL(gpio_num, level) gpio_set_level(gpio_num, level)

#   define MCPWM_GPIO_INIT(mcpwm_num, io_signal, gpio_num) mcpwm_gpio_init(mcpwm_num, io_signal, gpio_num)
#   define MCPWM_INIT(mcpwm_num, timer_num, mcpwm_conf) mcpwm_init(mcpwm_num, timer_num, mcpwm_conf)
#   define MCPWM_SET_DUTY(mcpwm_num, timer_num, gen, duty) mcpwm_set_duty(mcpwm_num, timer_num, gen, duty)
#   define MCPWM_SET_FREQUENCY(mcpwm_num, timer_num, frequency) mcpwm_set_frequency(mcpwm_num, timer_num, frequency)


#endif
