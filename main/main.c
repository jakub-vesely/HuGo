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
#include <i2c.h>
#include <display.h>

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

#include <Arduino.h>
#include <esp32-hal-gpio.h>
#include <mpu9250_lua_interface.h>


static const char *TAG = "HuGo";

#define IR_REMOTE_PIN GPIO_NUM_19

#define LEFT_FRONT_PIN GPIO_NUM_25
#define LEFT_BACK_PIN GPIO_NUM_26
#define RIGHT_FRONT_PIN GPIO_NUM_32
#define RIGHT_BACK_PIN GPIO_NUM_33

#define SDA_PIN GPIO_NUM_22
#define SCL_PIN GPIO_NUM_23

void app_main()
{
    printf("program start\n");

    hugo_lua_partition_init();

    lua_State* L = NULL;
    LUA_REOPEN(L);

    //Initialize arduino library to be possible to use it in the other libs
    initArduino();

    hugo_event_loop_init();
    /*if (!hugo_i2c_init(SDA_PIN, SCL_PIN)) {
        ESP_LOGW(TAG, "I2C init failed");
    }*/
    hugo_built_in_led_init_module(L);
    hugo_timer_init_module(L);
    hugo_gpio_init_module(L);
    hugo_chassis_init_module(L, LEFT_FRONT_PIN, LEFT_BACK_PIN, RIGHT_FRONT_PIN, RIGHT_BACK_PIN);
    hugo_ir_remote_init_module(L, IR_REMOTE_PIN);
    //hugo_display_init(L, true);

    hugo_mpu9250_init(L);

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
