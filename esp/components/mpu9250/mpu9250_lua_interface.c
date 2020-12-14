
#include "mpu9250_lua_interface.h"
#include <event_loop.h>
#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <timer.h>
#include <mpu9250_wrapper.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define FILL_TIMEOUT 10 * 100000

static const char *TAG = "mpu9250";
static lua_State *sL = NULL;

static float s_temperature = 0;
static float s_accel_x = 0;
static float s_accel_y = 0;
static float s_accel_z = 0;
static float s_gyro_x = 0;
static float s_gyro_y = 0;
static float s_gyro_z = 0;
static float s_mag_x = 0;
static float s_mag_y = 0;
static float s_mag_z = 0;

static int s_mpu9250_begin_event_id = -1;
static int s_fill_data_event_id = -1;
static int s_measured_event_id = -1;

static int s_fill_mask = MPU9250_FILL_NONE;
static int s_timer = -1;

static void _fill_timer_callback(void* arg){
    hugo_raise_event(EVENT_LOOP_TYPE_PERIPHERAL, s_fill_data_event_id, NULL, 0);
    hugo_start_timer_once(s_timer, FILL_TIMEOUT);
}

static void _mpu9250_begin_event_action(event_data_t _data, int _data_size){
    ESP_LOGI(TAG, "_mpu9250_begin_event_action");
    int status = mpu9250_begin();
    if (!status){
        ESP_LOGE(TAG, "mpu9250 initialization failed with status: %d", status);
        return;
    }

    ESP_LOGI(TAG, "mpu9250_begin OK");
    s_timer = hugo_create_new_timer(_fill_timer_callback);
    hugo_start_timer_once(s_timer, FILL_TIMEOUT);
}

static void _fill_data_event_action(event_data_t _data, int _data_size){
    mpu9250_read_sensor();
    if (s_fill_mask & MPU9250_FILL_TEMPERATURE){
        s_temperature = mpu9250_get_temperature();
    }
    if (s_fill_mask & MPU9250_FILL_ACCEL_X){
        s_accel_x = mpu9250_get_accel_x();
    }
    if (s_fill_mask & MPU9250_FILL_ACCEL_Y){
        s_accel_y = mpu9250_get_accel_y();
    }
    if (s_fill_mask & MPU9250_FILL_ACCEL_Z){
        s_accel_z = mpu9250_get_accel_z();
    }
    if (s_fill_mask & MPU9250_FILL_GYRO_X){
        s_gyro_x = mpu9250_get_gyro_x();
    }
    if (s_fill_mask & MPU9250_FILL_GYRO_Y){
        s_gyro_y = mpu9250_get_gyro_y();
    }
    if (s_fill_mask & MPU9250_FILL_GYRO_Z){
        s_gyro_z = mpu9250_get_gyro_z();
    }
    if (s_fill_mask & MPU9250_FILL_MAG_X){
        s_mag_x = mpu9250_get_mag_x();
    }
    if (s_fill_mask & MPU9250_FILL_MAG_Y){
        s_mag_y = mpu9250_get_mag_y();
    }
    if (s_fill_mask & MPU9250_FILL_MAG_Z){
        s_mag_z = mpu9250_get_mag_z();
    }

    hugo_raise_event(EVENT_LOOP_TYPE_PRIMARY, s_measured_event_id, NULL, 0);
}

static void _measured_action(event_data_t _data, int _data_size){
    lua_getglobal(sL, "mpu9250_measured_callback");
    if (lua_pcall(sL, 0, 0, 0))
    {
        ESP_LOGE(TAG, "mpu9250_measured_callback pcall failed: %s\n", lua_tostring(sL, -1));
    }
}

static int cl_mpu9250_get_temperature(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L,  s_temperature);
    return 1;
}

static int cl_mpu9250_get_accel_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_accel_x);
    return 1;
}

static int cl_mpu9250_get_accel_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_accel_y);
    //TODO: deleteme
    //vTaskDelay(50);
    return 1;
}

static int cl_mpu9250_get_accel_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_accel_z);
    return 1;
}

static int cl_mpu9250_get_gyro_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_gyro_x);
    return 1;
}

static int cl_mpu9250_get_gyro_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_gyro_y);
    return 1;
}

static int cl_mpu9250_get_gyro_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_gyro_z);
    return 1;
}

static int cl_mpu9250_get_mag_x(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_mag_x);
    return 1;
}

static int cl_mpu9250_get_mag_y(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_mag_y);
    return 1;
}

static int cl_mpu9250_get_mag_z(lua_State* L)
{
    LUA_PARAM_NR_CHECK(0);
    lua_pushnumber(L, s_mag_z);
    return 1;
}

static void _start(){
    ESP_LOGI(TAG, "mpu_start");
    s_mpu9250_begin_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PERIPHERAL);
    hugo_add_event_action(EVENT_LOOP_TYPE_PERIPHERAL, s_mpu9250_begin_event_id, _mpu9250_begin_event_action);

    s_fill_data_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PERIPHERAL);
    hugo_add_event_action(EVENT_LOOP_TYPE_PERIPHERAL, s_fill_data_event_id, _fill_data_event_action);

    hugo_raise_event(EVENT_LOOP_TYPE_PERIPHERAL, s_mpu9250_begin_event_id, NULL, 0);

    s_measured_event_id = hugo_get_new_event_id(EVENT_LOOP_TYPE_PRIMARY);
    hugo_add_event_action(EVENT_LOOP_TYPE_PRIMARY, s_measured_event_id, _measured_action);
}

bool hugo_mpu9250_init(lua_State* L, mpu9250_fill fill_mask)
{
    sL = L;
    s_fill_mask = fill_mask;

    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_temperature);

    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_accel_x);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_accel_y);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_accel_z);

    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_gyro_x);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_gyro_y);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_gyro_z);

    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_mag_x);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_mag_y);
    REGISTER_LUA_FUNCTION(L, cl_mpu9250_get_mag_z);

    _start();
    return true;
}
