/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "built_in_led.h"

#include <hugo_defines.h>
#include <gpio_abstraction.h>
#include <stdbool.h>

#define built_in_LED_GPIO 2

static bool initialized = false;

static void init()
{
    HUGO_GPIO_SET_PIN_FOR_OUT(built_in_LED_GPIO);
}

static int cl_built_in_led_change_state(lua_State *L)
{
    LUA_PARAM_NR_CHECK(1);

    if (!initialized)
    {
        init();
        initialized = true;
    }
    HUGO_GPIO_SET_PIN_LOGIC_VALUE(built_in_LED_GPIO, lua_toboolean(L, 1));
    return 0;
}

void hugo_built_in_led_init_module(lua_State* L)
{
    REGISTER_LUA_FUNCTION(L, cl_built_in_led_change_state);
}
