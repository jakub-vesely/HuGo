/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */
#include ".lua_files.h"
#include "external/lua/src/lua.h"
#include "external/lua/src/lualib.h"
#include "external/lua/src/lauxlib.h"
#include "hugo_defines.h"
#include "modules/built_in_led/built_in_led.h"
#include "modules/chassis/chassis.h"
#include "modules/gpio/gpio.h"
#include "modules/timer/timer.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_sleep.h>
#include <esp_spi_flash.h>
#include <esp_spiffs.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static const char *TAG = "HuGo";

/*
taken from file_serving example
*/
static esp_err_t init_partition(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/lua",
      .partition_label = NULL,
      .max_files = 64,   // This decides the maximum number of files that can be created on the storage
      .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ESP_FAIL;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}

static int cl_task_delay(lua_State* L)
{
    vTaskDelay(10);
    return 0;
}

void app_main()
{
    printf("program start\n");

    init_partition();
    create_lua_files();

    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);

    hugo_built_in_led_init_module(L);
    hugo_timer_init_module(L);
    hugo_gpio_init_module(L);
    hugo_chassis_init_module(L);

    REGISTER_LUA_FUNCTUIN(L, cl_task_delay);
    int status = luaL_dofile(L, "/lua/program.lua");
    if (status) {
        ESP_LOGE(TAG, "Lua Error: %s\n", lua_tostring(L, -1));
        return;
    }

    //this part shoul not be entered
    printf("%f\n", lua_tonumber(L, -1));

    lua_close(L);
    printf("Program end\n");
}
