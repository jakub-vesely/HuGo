/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include <hugo_defines.h>
#include <built_in_led.h>
#include <chassis.h>
#include <gpio.h>
#include <ir_remote.h>

#include <esp_err.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lua_partition.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <timer.h>
#include <unistd.h>
#include <event_loop.h>

static const char *TAG = "HuGo";

#define IR_REMOTE_PIN 19

#define LEFT_FRONT_PIN 25
#define LEFT_BACK_PIN 26
#define RIGHT_FRONT_PIN 32
#define RIGHT_BACK_PIN 33

void app_main()
{
    printf("program start\n");

    hugo_lua_partition_init();

    lua_State* L = NULL;
    LUA_REOPEN(L);

    hugo_event_loop_init();
    hugo_built_in_led_init_module(L);
    hugo_timer_init_module(L);
    hugo_gpio_init_module(L);
    hugo_chassis_init_module(L, LEFT_FRONT_PIN, LEFT_BACK_PIN, RIGHT_FRONT_PIN, RIGHT_BACK_PIN);
    hugo_ir_remote_init_module(L, IR_REMOTE_PIN);

    //REGISTER_LUA_FUNCTUIN(L, cl_task_delay);
    int status = luaL_dofile(L, "/lua/main.lua");
    if (status) {
        ESP_LOGE(TAG, "Lua Error: %s\n", lua_tostring(L, -1));
        return;
    }

    hugo_ir_remote_start_listening();
    hugo_process_events(false);

    //this part should never been reached
    LUA_CLOSE(L);
    printf("Program end\n");
}
