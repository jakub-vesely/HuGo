
#include "tiny_block_rgb_led.h"

#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>

//static const char *TAG = "rgb";
#define I2C_COMMAND_SET_RGB 64

static int cl_rgb_set_values(lua_State* L) {
    LUA_PARAM_NR_CHECK(4);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t data[4];
    data[0] = I2C_COMMAND_SET_RGB;
    data[1] = lua_tointeger(L, 2);
    data[2] = lua_tointeger(L, 3);
    data[3] = lua_tointeger(L, 4);
    hugo_i2c_write_data(address, data, 4);
    return 0;
}

bool tiny_rgb_led_init(lua_State* L) {
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_values);
    return true;
}
