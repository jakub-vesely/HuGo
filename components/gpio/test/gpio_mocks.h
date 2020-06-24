#pragma once

#include <fff.h>
#include <driver/gpio.h>
#include <driver/mcpwm.h>

DECLARE_FAKE_VOID_FUNC(fff_gpio_pad_select_gpio, uint8_t);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_gpio_set_direction, gpio_num_t, gpio_mode_t);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_gpio_set_level, gpio_num_t, uint32_t);

DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_mcpwm_gpio_init, mcpwm_unit_t, mcpwm_io_signals_t, int);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_mcpwm_init, mcpwm_unit_t, mcpwm_timer_t, const mcpwm_config_t *);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_mcpwm_set_duty, mcpwm_unit_t, mcpwm_timer_t, mcpwm_generator_t, float);
DECLARE_FAKE_VALUE_FUNC(esp_err_t, fff_mcpwm_set_frequency, mcpwm_unit_t, mcpwm_timer_t, uint32_t);

