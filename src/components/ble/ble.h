/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <lua.h>

typedef bool (*command_callback_t)(
    uint8_t command_id, uint8_t const * in_data, size_t in_data_size, uint8_t ** out_data, size_t *out_data_size
);

bool hugo_ble_initialize(lua_State* L, char * device_name_prefix, command_callback_t command_callback);
