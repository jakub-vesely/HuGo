
#include "tiny_block_rgb_led.h"

#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <hugo_tiny_block_defines.h>
#include "tiny_block_rgb_led_arduino/tiny_block_rgb_led_arduino.h"

#include <i2c.h>

//static const char *TAG = "rgb";

static int cl_rgb_set_values(lua_State* L) {
    LUA_PARAM_NR_CHECK(4);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t data[3];
    data[0] = lua_tointeger(L, 2);
    data[1] = lua_tointeger(L, 3);
    data[2] = lua_tointeger(L, 4);
    hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_RGB, I2C_COMMAND_RGB_SET_RGB, data, 3, NULL, 0);
    return 0;
}

static int cl_rgb_set_on(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t address = lua_tointeger(L, 1);
    hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_RGB, I2C_COMMAND_RGB_SET_ON, NULL, 0, NULL, 0);
    return 0;
}

static int cl_rgb_set_off(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t address = lua_tointeger(L, 1);
    hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_RGB, I2C_COMMAND_RGB_SET_OFF, NULL, 0, NULL, 0);
    return 0;
}

static int cl_rgb_set_color(lua_State* L) {
    LUA_PARAM_NR_CHECK(2);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t color = lua_tointeger(L, 2);
    hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_RGB, I2C_COMMAND_RGB_SET_COLOR, &color, 1, NULL, 0);
    return 0;
}

bool tiny_rgb_led_init(lua_State* L) {
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_values);
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_on);
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_off);
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_color);
    return true;
}
