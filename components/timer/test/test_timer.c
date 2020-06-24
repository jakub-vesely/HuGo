#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lua.h>
#include <stdbool.h>
#include <timer.c>
#include <unity.h>

#define TEST_TAG "[timer]"

TEST_CASE("timer_main_test", TEST_TAG)
{
    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);

    hugo_timer_init_module(L);

    //start timer from Lua
    CHECK_LUA_DOSTRING(L,
        "called = false;"
        "function was_called() return called end;"
        "timer = require 'timer';"
        "timer.call_after_time(function() called = true end, 0.01);"
    );

    int counter = 20;
    while(--counter > 0)
    {
        vTaskDelay(1);
    }

    CHECK_LUA_GET_GLOBAL(L, "was_called", 0, 1, 0);
    TEST_ASSERT_EQUAL(lua_toboolean(L, -1), true);

    lua_close(L);
}
