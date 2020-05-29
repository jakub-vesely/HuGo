/*
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_spiffs.h"
#include "esp_err.h"
#include "esp_log.h"

#include "external/lua/src/lua.h"
#include "external/lua/src/lualib.h"
#include "external/lua/src/lauxlib.h"
#include ".lua_files.h"

#include "modules/adapters/build_in_led_adapter.h"

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
      .max_files = 100,   // This decides the maximum number of files that can be created on the storage
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

void app_main()
{
    printf("program start\n");

    init_partition();
    create_lua_files();

    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);
    create_build_in_led_adapter(L);

    int status = luaL_dofile(L, "/lua/program.lua");
    if (status) {
        ESP_LOGE(TAG, "Couldn't load file: %s\n", lua_tostring(L, -1));
        return;
    }
    printf("%f\n", lua_tonumber(L, -1));

    lua_close(L);

    printf("Program end\n");
}
