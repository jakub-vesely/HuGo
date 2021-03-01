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
#include <rgb_led.h>
#include <motor_block.h>

static const char *TAG = "HuGo";

#define IR_REMOTE_PIN GPIO_NUM_19

#define LEFT_FRONT_PIN GPIO_NUM_25
#define LEFT_BACK_PIN GPIO_NUM_26
#define RIGHT_FRONT_PIN GPIO_NUM_32
#define RIGHT_BACK_PIN GPIO_NUM_33

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

static void _peripheral_event_loop_task(void *data) {
    ESP_LOGI(TAG, "_peripheral_event_loop_task started");
    hugo_event_loop_init(EVENT_LOOP_TYPE_PERIPHERAL);
    if (!hugo_i2c_init(SDA_PIN, SCL_PIN)) {
        ESP_LOGW(TAG, "I2C init failed");
    }
    hugo_process_events(EVENT_LOOP_TYPE_PERIPHERAL, false);
}

void app_main()
{
    printf("program start\n");

    hugo_lua_partition_init();

    lua_State* L = NULL;
    LUA_REOPEN(L);

    //Initialize arduino library to be possible to use it in the other libs
    initArduino();

    hugo_event_loop_init(EVENT_LOOP_TYPE_PRIMARY);
    //peripheral event loop is performed in a task
    xTaskCreate(&_peripheral_event_loop_task, "_peripheral_event_loop_task",  4096, (void *)NULL, 2, NULL);

    hugo_built_in_led_init_module(L);
    hugo_timer_init_module(L);
    hugo_gpio_init_module(L);
    //hugo_chassis_init_module(L, LEFT_FRONT_PIN, LEFT_BACK_PIN, RIGHT_FRONT_PIN, RIGHT_BACK_PIN);
    //hugo_ir_remote_init_module(L, IR_REMOTE_PIN);
    //hugo_mpu9250_init(L, MPU9250_FILL_ACCEL_Y | MPU9250_FILL_GYRO_Z);
    //hugo_display_init(L, true);
    hugo_rgb_led_init(L);
    hugo_motor_block_init(L);

    //REGISTER_LUA_FUNCTION(L, cl_task_delay);
    int status = luaL_dofile(L, "/lua/main.lua");
    if (status) {
        ESP_LOGE(TAG, "Lua Error: %s\n", lua_tostring(L, -1));
        return;
    }

    //hugo_ir_remote_start_listening();
    hugo_process_events(EVENT_LOOP_TYPE_PRIMARY, false);

    //this part should never been reached
    LUA_CLOSE(L);
    printf("Program end\n");
}
