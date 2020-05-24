/*
Copyright (c) 2020 jakub-vesely
This software is published under MIT license. Full text of the licence is available on https://opensource.org/licenses/MIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"
#include "esp_log.h"

#include "external/lua/src/lua.h"
#include "external/lua/src/lualib.h"
#include "external/lua/src/lauxlib.h"

static const char *TAG = "HuGo";

// Handle of the wear levelling library instance
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

int print_hello(lua_State *L)
{
    printf("hello");
    return 1;
}

void init_file(const char * file_name, const char * data_start, const char * data_end)
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

void init_partition()
{
    esp_vfs_fat_mount_config_t mount_config;
    mount_config.max_files = 4;
    mount_config.format_if_mount_failed = true;
    mount_config.allocation_unit_size = CONFIG_WL_SECTOR_SIZE;

    esp_err_t err = esp_vfs_fat_spiflash_mount("/scripts", "storage", &mount_config, &s_wl_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }

    // The file is embedded as a binary data. Create a file first containing the embedded
    extern const char data_start[] asm("_binary_test_lua_start");
    extern const char data_end[]   asm("_binary_test_lua_end");
    init_file("/scripts/test.lua", data_start, data_end);

    extern const char data2_start[] asm("_binary_test2_lua_start");
    extern const char data2_end[]   asm("_binary_test2_lua_end");
    init_file("/scripts/test2.lua", data2_start, data2_end);

    ESP_LOGI(TAG, "data written");
}

void app_main()
{
    printf("program start\n");

    init_partition();

    lua_State* L = (lua_State*)luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "print_hello", print_hello);


    int status = luaL_dostring(L, "print_hello(); print(' Lua string'); return 1 ");
    if (status) {
        ESP_LOGE(TAG, "Couldn't execute string: %s\n", lua_tostring(L, -1));
        return;
    }
    printf("%f\n", lua_tonumber(L, -1));

    status = luaL_dofile(L, "/scripts/test.lua");
    if (status) {
        ESP_LOGE(TAG, "Couldn't load file: %s\n", lua_tostring(L, -1));
        return;
    }
    printf("%f\n", lua_tonumber(L, -1));

    lua_close(L);
    printf("Program end\n");
}
