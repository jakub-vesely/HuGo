/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "tiny_block_base.h"
#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>
#include <hugo_tiny_block_defines.h>

static const char *TAG = "tiny_block_base";
static int cl_tiny_base_set_i2c_address(lua_State* L)
{
    LUA_PARAM_NR_CHECK(2);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t new_address = lua_tointeger(L, 2);
    uint8_t data[2];
    data[0] = I2C_COMMAND_CHANGE_I2C_ADDRESS;
    data[1] = new_address;
    if (!hugo_i2c_write_data(address, data, 2)){
        ESP_LOGE(TAG, "i2c address was not changed - for %d", address);
    }

    return 0;
}

bool tiny_block_base_init(lua_State* L){
    REGISTER_LUA_FUNCTION(L, cl_tiny_base_set_i2c_address);
    return true;
}