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

#include "modules/adapters/build_in_led_adapter.h"

static const char *TAG = "HuGo";

//#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + CONFIG_SPIFFS_OBJ_NAME_LEN)
//#define MAX_FILE_SIZE   (100*1024) // 100 KB
//#define MAX_FILE_SIZE_STR "100KB"


void init_interfaces_dir()
{
    char const dit_path[] = "/lua/interfcsfaces";
    struct stat st = {0};

    if (stat(dit_path, &st) == -1) {
        ESP_LOGI(TAG, "creating dir %s", dit_path);
        if (0 == mkdir(dit_path, 0777))
            ESP_LOGE(TAG, "dir %s was not created", dit_path);
        else
             ESP_LOGI(TAG, "dir %s was created successfully", dit_path);
    }
    else{
        ESP_LOGI(TAG, "dir %s already exits", dit_path);
    }

}
void init_single_file(const char * file_name, const char * data_start, const char * data_end)
{
    if( access(file_name, F_OK ) != -1 ) {
        ESP_LOGI(TAG, "file %s already exists", file_name);
    }
    else{
        ESP_LOGI(TAG, "file %s will be created", file_name);
        FILE *f = fopen(file_name, "wb");

        if (f == NULL) {
            ESP_LOGE(TAG, "Failed to open file for writing");
            return;
        }
        fwrite(data_start, sizeof(char), data_end - data_start - 1, f);
        fclose(f);
    }
}

void init_files()
{

    // The file is embedded as a binary data. Create a file first containing the embedded
    extern const char data_start[] asm("_binary_build_in_led_int_lua_start");
    extern const char data_end[]   asm("_binary_build_in_led_int_lua_end");
    init_single_file("/lua/interfcs/build_in_led_int.lua", data_start, data_end);

    extern const char data2_start[] asm("_binary_build_in_led_beh_lua_start");
    extern const char data2_end[]   asm("_binary_build_in_led_beh_lua_end");
    init_single_file("/lua/build_in_led_beh.lua", data2_start, data2_end);

    ESP_LOGI(TAG, "data written");
}

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
    //init_interfaces_dir();
    init_files();

    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);
    create_build_in_led_adapter(L);

    int status = luaL_dofile(L, "/lua/build_in_led_beh.lua");
    if (status) {
        ESP_LOGE(TAG, "Couldn't load file: %s\n", lua_tostring(L, -1));
        return;
    }
    printf("%f\n", lua_tonumber(L, -1));

    lua_close(L);

    printf("Program end\n");
}
