#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lua.h>
#include <stdbool.h>
#include <timer.c>
#include <unity.h>

#define TEST_TAG "[timer]"

TEST_CASE("timer_main_test", TEST_TAG)
{
    // start Lua
    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);

    // initialize timer module
    hugo_timer_init_module(L);

    //start timer from Lua
    int status = luaL_dostring(L,
        "called = false;"
        "function was_called() return called end;"
        "timer = require 'timer';"
        "timer.call_after_time(function() called = true end, 0.01);"
    );
    if (status) {
        ESP_LOGE(TAG, "Lua Error: %s\n", lua_tostring(L, -1));
    }
    TEST_ASSERT_EQUAL(status, 0);

    int counter = 20;
    while(--counter > 0)
    {
        vTaskDelay(1);
    }

    lua_getglobal(sL, "was_called");
    status = lua_pcall(sL, 0, 1, 0);
    if (status)
    {
        ESP_LOGE(TAG, "was_called pcall failed: %s\n", lua_tostring(sL, -1));
    }
    TEST_ASSERT_EQUAL(status, 0);
    TEST_ASSERT_EQUAL(lua_toboolean(L, -1), true);

    // stop Lua
    lua_close(L);
}
