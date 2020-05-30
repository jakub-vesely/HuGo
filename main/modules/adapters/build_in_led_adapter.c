/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
 */

#include "build_in_led_adapter.h"
#include "driver/gpio.h"
#include "hugo_error.h"
#include <stdbool.h>

#define BUILD_IN_LED_GPIO 2

static bool initialized = false;

static void init()
{
    /* Configure the IOMUX register for pad BLINK_GPIO (some pads are
       mixed to GPIO on reset already, but some default to other
       functions and need to be switched to GPIO. Consult the
       Technical Reference for a list of pads and their default
       functions.)
    */
    gpio_pad_select_gpio(BUILD_IN_LED_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BUILD_IN_LED_GPIO, GPIO_MODE_OUTPUT);
}

static HUGO_RET_VAL change_state(lua_State *L)
{
    //process input argument
    int n = lua_gettop(L);
    if(n != 1)
    {
        lua_pushstring(L, "buildin_led_adapter.change_state: unexpected number of arguments (expected just 1)");
        lua_error(L);
    }

    int on = lua_tointeger(L, 1);
    if (!initialized)
    {
        init();
        initialized = true;
    }

    return (ESP_OK == gpio_set_level(BUILD_IN_LED_GPIO, on)? HUGO_OK : HUGO_FAIL);
}

void create_build_in_led_adapter(lua_State* L)
{
    lua_register(L, "c_build_in_led_change_state", change_state);
}
