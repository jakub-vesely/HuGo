
#include "tiny_block_ir_receiver.h"

#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>
#include <string.h>
#include <hugo_tiny_block_defines.h>
#include "tiny_block_ir_arduino/tiny_block_ir_arduino.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "IR";

static uint8_t s_data[4];

static int cl_get_ir_data(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t address = lua_tointeger(L, 1);

    uint8_t data_ready = 0;
    int32_t code = 0;
    if (!hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_IR, I2C_COMMAND_GET_IR_DATA_READY, NULL, 0, &data_ready, 1)){
        data_ready = 0;
        ESP_LOGE(TAG, "no IR data size provided - for %d", address);
    }
    if (data_ready != 0){
        uint8_t data[3] = {0,0,0};
        if (!hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_IR, I2C_COMMAND_GET_IR_DATA, NULL, 0, data, 3)){
            ESP_LOGE(TAG, "no IR data size provided - for %d", address);
        }
        else{
            code = data[0] | data[1] << 8 | data[2] << 16;
            //ESP_LOGI(TAG, "received code %d", code);
        }
    }

    lua_pushinteger(L, code);
    return 1;
}

bool tiny_ir_receiver_init(lua_State* L) {
    REGISTER_LUA_FUNCTION(L, cl_get_ir_data);
    return true;
}
