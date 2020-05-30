/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
 */

#include "timer.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "external/lua/src/lualib.h"
#include "external/lua/src/lauxlib.h"

#define TIMER_ARRAY_SIZE 10

static const char *TAG = "Timer";
static lua_State *sL = NULL;

typedef struct
{
    int32_t id;
    esp_timer_handle_t handle;
} timer_item;

static timer_item timer_items[TIMER_ARRAY_SIZE];
void store_timer_item(int32_t id, esp_timer_handle_t handle)
{
    for (int i = 0; i < TIMER_ARRAY_SIZE; i++)
    {
        if (timer_items[i].id == 0)
        {
            timer_items[i].id = id;
            timer_items[i].handle = handle;
            return;
        }
    }
    ESP_LOGE(TAG, "timer buffer is full");
}

void remove_timer_item(int32_t id)
{
    for (int i = 0; i < TIMER_ARRAY_SIZE; i++)
    {
        if (timer_items[i].id == id)
        {
            ESP_ERROR_CHECK(esp_timer_delete(timer_items[i].handle));
            timer_items[i].id = 0;
            return;
        }
    }
}

static void timer_callback(void* arg)
{
    int id = (int) arg;
    remove_timer_item(id); //it is not necessary any more can be removed to be place for another timer if is creaded in callback

    if (luaL_loadfile(sL, "/lua/timer.lua")) /* Load but don't run the Lua script */
        ESP_LOGE(TAG, "timer_int load file failed: %s\n", lua_tostring(sL, -1));

    lua_getglobal(sL, "Timer_callback_collector");
    lua_pushinteger(sL, id);
    if (lua_pcall(sL, 1, 0, 0))
        ESP_LOGE(TAG, "timer_callback pcall failed: %s\n", lua_tostring(sL, -1));
}

static int call_after_time(lua_State *L)
{
    //process input argument
    int n = lua_gettop(L);
    if(n != 2)
    {
        lua_pushstring(L, "timer.call_after: unexpected number of arguments (expected 2)");
        lua_error(L);
    }

    int32_t id = lua_tointeger(L, 1);
    int32_t time_us = lua_tointeger(L, 2);

    const esp_timer_create_args_t timer_args = {
            .callback = &timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "timer",
            .arg = (void*) id,
    };

    esp_timer_handle_t timer;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
            store_timer_item(id, timer);
    ESP_ERROR_CHECK(esp_timer_start_once(timer, time_us));

    return 0;
}

void init_timer_module(lua_State* L)
{
    for (int i = 0; i < TIMER_ARRAY_SIZE; i++)
    {
        timer_items[i].id= 0;
    }

    sL = L;
    lua_register(L, "c_timer_call_after", call_after_time);
}
