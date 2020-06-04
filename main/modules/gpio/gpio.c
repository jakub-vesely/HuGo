/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "esp_log.h"
#include <driver/mcpwm.h>
#include <hugo_defines.h>

#include  <external/lua/src/lualib.h>
#include <external/lua/src/lauxlib.h>

void hugo_gpio_set_pin_for_out(uint8_t pin)
{
    /* Configure the IOMUX register for the pad (some pads are
       mixed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO.)
    */
    gpio_pad_select_gpio(pin);
    ESP_ERROR_CHECK(gpio_set_direction(pin, GPIO_MODE_OUTPUT));
}

void hugo_gpio_set_pin_logic_value(uint8_t pin, bool value)
{
    ESP_ERROR_CHECK(gpio_set_level(pin, value));
}

void hugo_gpio_set_pin_for_pwm(int pin, uint32_t frequency, float duty)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, pin);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = frequency;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = duty;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0.0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config));    //Configure PWM0A & PWM0B with above settings
}

void hugo_gpio_set_pwm_duty(float duty)
{
    ESP_ERROR_CHECK(mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty));
}

void hugo_gpio_set_pwm_frequency(int frequency)
{
    ESP_ERROR_CHECK(mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, frequency));
}

static int cl_gpio_set_pin_for_out(lua_State* L)
{
    LUA_PARAM_NR_CHECK(1);
    hugo_gpio_set_pin_for_out(lua_tointeger(L, 1));
    return 0;
}

static int cl_gpio_set_pin_logic_value(lua_State* L)
{
    LUA_PARAM_NR_CHECK(2);
    hugo_gpio_set_pin_logic_value(lua_tointeger(L, 1), lua_tointeger(L, 2));
    return 0;
}

static int cl_gpio_set_pin_for_pwm(lua_State* L)
{
    LUA_PARAM_NR_CHECK(3);
    hugo_gpio_set_pin_for_pwm(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_tonumber(L, 3));
    return 0;
}

static int cl_gpio_set_pwm_frequency(lua_State* L)
{
    LUA_PARAM_NR_CHECK(1);
    hugo_gpio_set_pwm_frequency(lua_tointeger(L, 1));
    return 0;
}

static int cl_gpio_set_pwm_duty(lua_State* L)
{
    LUA_PARAM_NR_CHECK(1);
    hugo_gpio_set_pwm_duty(lua_tonumber(L, 1));
    return 0;
}

void hugo_gpio_init_module(lua_State* L)
{
    REGISTER_LUA_FUNCTUIN(L, cl_gpio_set_pin_for_out);
    REGISTER_LUA_FUNCTUIN(L, cl_gpio_set_pin_logic_value);
    REGISTER_LUA_FUNCTUIN(L, cl_gpio_set_pin_for_pwm);
    REGISTER_LUA_FUNCTUIN(L, cl_gpio_set_pwm_frequency);
    REGISTER_LUA_FUNCTUIN(L, cl_gpio_set_pwm_duty);
}
