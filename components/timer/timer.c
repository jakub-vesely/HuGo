/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "timer.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <hugo_defines.h>
#include <lualib.h>
#include <lauxlib.h>
#include <hugo_defines.h>
#include <stdbool.h>
#include <stddef.h>

#define TIMER_ARRAY_SIZE 10

static const char *TAG = "Timer";
static lua_State *sL = NULL;
static int32_t s_timer_counter = 0;

typedef struct {
    int32_t id;
    esp_timer_handle_t handle;
} timer_item;

static timer_item s_timer_items[TIMER_ARRAY_SIZE];
static bool create_timer_item(int32_t timer_id, esp_timer_handle_t handle) {
    for (int i = 0; i < TIMER_ARRAY_SIZE; i++) {
        if (s_timer_items[i].id == 0) {
            s_timer_items[i].id = timer_id;
            s_timer_items[i].handle = handle;
            return true;
        }
    }
    ESP_LOGE(TAG, "timer buffer is full");
    return false;
}

void remove_timer_item(int32_t id) {
    for (int i = 0; i < TIMER_ARRAY_SIZE; i++) {
        if (s_timer_items[i].id == id) {
            ESP_ERROR_CHECK(esp_timer_delete(s_timer_items[i].handle));
            s_timer_items[i].id = 0;
            return;
        }
    }
}

static void timer_callback(void* arg) {
    int id = (int)arg;
    remove_timer_item(id); //it is not necessary any more can be removed to be place for another timer if is creaded in callback

    if (luaL_loadfile(sL, "/lua/timer.lua")) { /* Load but don't run the Lua script */
        ESP_LOGE(TAG, "timer_int load file failed: %s\n", lua_tostring(sL, -1));
    }

    lua_getglobal(sL, "Timer_callback_collector");
    lua_pushinteger(sL, id);
    if (lua_pcall(sL, 1, 0, 0)) {
        ESP_LOGE(TAG, "timer_callback pcall failed: %s\n", lua_tostring(sL, -1));
    }
}

int hugo_create_new_timer(esp_timer_cb_t callback_function) {
    int32_t timer_id = ++s_timer_counter;
    const esp_timer_create_args_t timer_args = {
            .callback = callback_function,
            /* name is optional, but may help identify the timer when debugging */
            .name = "timer",
            .arg = (void*) timer_id,
    };

    esp_timer_handle_t handle;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &handle));
    create_timer_item(timer_id, handle);
    return timer_id;
}

static int cl_create_new_timer(lua_State *L) {
    LUA_PARAM_NR_CHECK(0);

    lua_pushinteger(L, hugo_create_new_timer(timer_callback));
    return 1;
}

bool hugo_start_timer(int timer_id, int time_us) {
    for (int i = 0; i < TIMER_ARRAY_SIZE; ++i) {
        if (s_timer_items[i].id == timer_id) {
            ESP_ERROR_CHECK(esp_timer_start_once(s_timer_items[i].handle, time_us));
            return true;
        }
    }
    return false;
}

static int cl_start_timer(lua_State *L) {
    LUA_PARAM_NR_CHECK(2);
    int32_t timer_id = lua_tointeger(L, 1);
    int32_t time_us = lua_tointeger(L, 2);

    lua_pushinteger(L, hugo_start_timer(timer_id, time_us));
    return 1;
}

void hugo_timer_init_module(lua_State* L) {
    sL = L;

    for (int i = 0; i < TIMER_ARRAY_SIZE; i++) {
        s_timer_items[i].id= 0;
    }

    REGISTER_LUA_FUNCTION(L, cl_create_new_timer);
    REGISTER_LUA_FUNCTION(L, cl_start_timer);
}
