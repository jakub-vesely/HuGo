/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include ".lua_files.h"
#include <esp_log.h>
#include <esp_spi_flash.h>
#include <esp_spiffs.h>
#include <stdbool.h>
#include <lua_partition.h>

#define TAG "LuaPartition"
#define MAIN_FILE_PATH LUA_PARTITION_PATH "/main.lua"

static bool is_lua_partition_initialized(){
    //erasing of flash is expected before each flashing - if ESP is flashed files are not present, if just starts, files  are present and doesnt have to be rewriten
    //it is expected that main.lua is moved as last one
    FILE *file;
    if ((file = fopen(MAIN_FILE_PATH, "r"))){
        fclose(file);
        return true;
    }
    return false;
}

/*
taken from file_serving example
*/
bool hugo_lua_partition_init(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = LUA_PARTITION_PATH,
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
        return false;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);

    if (!is_lua_partition_initialized()){
        create_lua_files();
    }
    return true;
}
