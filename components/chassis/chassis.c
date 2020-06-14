/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */
#include <hugo_defines.h>
#include <gpio.h>
#include <stdbool.h>
#include <stdio.h>
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

#define PIN_LEFT_A 25
#define PIN_LEFT_B 26

#define PIN_RIGHT_A 32
#define PIN_RIGHT_B 33

static void set_combination(bool left_a, bool left_b, bool right_a, bool right_b)
{
    hugo_gpio_set_pin_logic_value(PIN_LEFT_A, left_a);
    hugo_gpio_set_pin_logic_value(PIN_LEFT_B, left_b);
    hugo_gpio_set_pin_logic_value(PIN_RIGHT_A, right_a);
    hugo_gpio_set_pin_logic_value(PIN_RIGHT_B, right_b);

}
static int cl_chassis_stop(lua_State* L)
{
    printf("stop");
    LUA_PARAM_NR_CHECK(0);
    set_combination(false, false, false, false);
    return 0;
}

static int cl_chassis_go_forward(lua_State* L)
{
    printf("forward");
    LUA_PARAM_NR_CHECK(0);
    set_combination(true, false, true, false);
    return 0;
}

static int cl_chassis_go_backward(lua_State* L)
{
    printf("backkward");
    LUA_PARAM_NR_CHECK(0);
    set_combination(false, true, false, true);
    return 0;
}

static int cl_chassis_rotate_clockwise(lua_State* L)
{
    printf("clockwise");
    LUA_PARAM_NR_CHECK(0);
    set_combination(false, true, true, false);
    return 0;
}

static int cl_chassis_rotate_counterclockwise(lua_State* L)
{
    printf("counterclockwise");
    LUA_PARAM_NR_CHECK(0);
    set_combination(true, false, false, true);
    return 0;
}

void hugo_chassis_init_module(lua_State* L)
{
    hugo_gpio_set_pin_for_out(PIN_LEFT_A);
    hugo_gpio_set_pin_for_out(PIN_LEFT_B);
    hugo_gpio_set_pin_for_out(PIN_RIGHT_A);
    hugo_gpio_set_pin_for_out(PIN_RIGHT_B);

    REGISTER_LUA_FUNCTUIN(L, cl_chassis_stop);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_go_forward);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_go_backward);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_rotate_clockwise);
    REGISTER_LUA_FUNCTUIN(L, cl_chassis_rotate_counterclockwise);

}
