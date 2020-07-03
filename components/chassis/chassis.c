/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */
#include <hugo_defines.h>
#include <gpio.h>
#include <stdbool.h>
#include <stdio.h>
#include <gpio_abstraction.h>

/* HuGo has 4 driven wheels (two motors are placed on each side)
 * rotation direction is managed separately on each side but both motor on each side are managed together.
 * Motors are powered over H-Bridge with 2 input lines (here are named A and B)
 * direction of rotation is based on following logic
 *
 * A | B | direction
 * -----------------
 * 0 | 0 | -
 * 1 | 0 | forward
 * 0 | 1 | backward
 * 1 | 1 | -
 *
 * when direction of rotation are the same on both sides chassis move forward/backward. When they are different it rotates left/right
 */

static int _left_front_pin  = 0;
static int _left_back_pin = 0;
static int _right_front_pin = 0;
static int _right_back_pin = 0;

static void _set_combination(bool left_front_level, bool left_back_level, bool right_front_level, bool right_back_level)
{
    HUGO_GPIO_SET_PIN_LOGIC_VALUE(_left_front_pin, left_front_level);
    HUGO_GPIO_SET_PIN_LOGIC_VALUE(_left_back_pin, left_back_level);
    HUGO_GPIO_SET_PIN_LOGIC_VALUE(_right_front_pin, right_front_level);
    HUGO_GPIO_SET_PIN_LOGIC_VALUE(_right_back_pin, right_back_level);
}

static int cl_chassis_stop(lua_State* L)
{
    printf("stop\n");
    LUA_PARAM_NR_CHECK(0);
    _set_combination(false, false, false, false);
    return 0;
}

static int cl_chassis_go_forward(lua_State* L)
{
    printf("forward\n");
    LUA_PARAM_NR_CHECK(0);
    _set_combination(true, false, true, false);
    return 0;
}

static int cl_chassis_go_backward(lua_State* L)
{
    printf("backkward\n");
    LUA_PARAM_NR_CHECK(0);
    _set_combination(false, true, false, true);
    return 0;
}

static int cl_chassis_rotate_clockwise(lua_State* L)
{
    printf("clockwise\n");
    LUA_PARAM_NR_CHECK(0);
    _set_combination(false, true, true, false);
    return 0;
}

static int cl_chassis_rotate_counterclockwise(lua_State* L)
{
    printf("counterclockwise\n");
    LUA_PARAM_NR_CHECK(0);
    _set_combination(true, false, false, true);
    return 0;
}

void hugo_chassis_init_module(lua_State* L, int left_front_pin, int left_back_pin, int right_front_pin, int right_back_pin)
{
    _left_front_pin = left_front_pin;
    _left_back_pin = left_back_pin;
    _right_front_pin = right_front_pin;
    _right_back_pin = right_back_pin;

    hugo_gpio_set_pin_for_out(left_front_pin);
    hugo_gpio_set_pin_for_out(left_back_pin);
    hugo_gpio_set_pin_for_out(right_front_pin);
    hugo_gpio_set_pin_for_out(right_back_pin);

    REGISTER_LUA_FUNCTUIN(L, cl_chassis_stop);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_go_forward);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_go_backward);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_rotate_clockwise);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_rotate_counterclockwise);
}
