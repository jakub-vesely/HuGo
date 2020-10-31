
#include "mpu9250_lua_interface.h"
#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>

#include <mpu9250_wrapper.h>

static const char *TAG = "mpu9250";

static int cl_mpu9250_read_sensor(lua_State* L)
{
    mpu9250_read_sensor();
    return 0;
}

static int cl_mpu9250_get_temperature(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L,  mpu9250_get_temperature());
    return 1;
}

static int cl_mpu9250_get_accel_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_accel_x());
    return 1;
}

static int cl_mpu9250_get_accel_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_accel_y());
    return 1;
}

static int cl_mpu9250_get_accel_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_accel_z());
    return 1;
}

static int cl_mpu9250_get_gyro_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_gyro_x());
    return 1;
}

static int cl_mpu9250_get_gyro_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_gyro_y());
    return 1;
}

static int cl_mpu9250_get_gyro_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_gyro_z());
    return 1;
}

static int cl_mpu9250_get_mag_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_mag_x());
    return 1;
}

static int cl_mpu9250_get_mag_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_mag_y());
    return 1;
}

static int cl_mpu9250_get_mag_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, mpu9250_get_mag_z());
    return 1;
}

bool hugo_mpu9250_init(lua_State* L)
{
    int status = mpu9250_begin();
    if (status != 1)
    {
        ESP_LOGE(TAG, "mpu9250 initialization failed with status: %d", status);
        return false;
    }

    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_read_sensor);

    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_temperature);

    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_accel_x);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_accel_y);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_accel_z);

    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_gyro_x);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_gyro_y);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_gyro_z);

    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_mag_x);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_mag_y);
    REGISTER_LUA_FUNCTUIN(L, cl_mpu9250_get_mag_z);

    return true;
}
