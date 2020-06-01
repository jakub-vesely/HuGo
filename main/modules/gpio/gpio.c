/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
 */

#include "esp_log.h"
#include "driver/mcpwm.h"

#include "external/lua/src/lualib.h"
#include "external/lua/src/lauxlib.h"

int init_pin_for_out(lua_State* L)
{
    int n = lua_gettop(L);
    if(n != 1)
    {
        lua_pushstring(L, "gpio.init_pin_for_out: unexpected number of arguments (expected 1)");
        lua_error(L);
    }
    int32_t pin = lua_tointeger(L, 1);

    gpio_pad_select_gpio(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    return 0;
}

int set_logic_value(lua_State* L)
{
    int n = lua_gettop(L);
    if(n != 2)
    {
        lua_pushstring(L, "gpio.set_logic_value: unexpected number of arguments (expected 2)");
        lua_error(L);
    }
    int32_t pin = lua_tointeger(L, 1);
    int32_t value = lua_tointeger(L, 2);
    gpio_set_level(pin, value);
    return 0;
}

int init_pin_for_pwm(lua_State* L)
{
    int n = lua_gettop(L);
    if(n != 3)
    {
        lua_pushstring(L, "gpio.init_pin_for_pwm: unexpected number of arguments (expected 3)");
        lua_error(L);
    }
    int32_t pin = lua_tointeger(L, 1);
    int32_t frequency = lua_tointeger(L, 2);
    int32_t duty = lua_tointeger(L, 3);

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, pin);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = frequency;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = duty;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0.0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
    return 0;
}

int set_pwm_frequency(lua_State* L)
{
    return 0;
}

int set_pwm_duty(lua_State* L)
{
    int n = lua_gettop(L);
    if(n != 1)
    {
        lua_pushstring(L, "gpio.set_pwm_duty: unexpected number of arguments (expected 1)");
        lua_error(L);
    }
    int32_t duty = lua_tointeger(L, 1);
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty);
    return 0;
}

void init_gpio_module(lua_State* L)
{
    lua_register(L, "c_init_pin_for_out", init_pin_for_out);
    lua_register(L, "c_set_logic_value", set_logic_value);

    lua_register(L, "c_init_pin_for_pwm", init_pin_for_pwm);
    lua_register(L, "c_set_pwm_frequency", set_pwm_frequency);
    lua_register(L, "c_set_pwm_duty", set_pwm_duty);
}
