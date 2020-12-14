
#include "rgb_led.h"

#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>

//static const char *TAG = "rgb";

static int cl_rgb_set_values(lua_State* L) {
    LUA_PARAM_NR_CHECK(4);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t data[3];
    data[0] = lua_tointeger(L, 2);
    data[1] = lua_tointeger(L, 3);
    data[2] = lua_tointeger(L, 4);
    hugo_i2c_write_data(address, data, 3);
    return 0;
}

bool hugo_rgb_led_init(lua_State* L) {
    REGISTER_LUA_FUNCTION(L, cl_rgb_set_values);
    return true;
}
