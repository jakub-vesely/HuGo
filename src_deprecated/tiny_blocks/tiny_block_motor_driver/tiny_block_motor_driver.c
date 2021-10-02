/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#include "tiny_block_motor_driver_arduino/motor_commands.h"
#include "tiny_block_motor_driver.h"
#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>
#include <hugo_tiny_block_defines.h>

static const char *TAG = "motor";

static void _send_command(lua_State* L, uint8_t command){
    LUA_PARAM_NR_CHECK(2);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t motor_id = lua_tointeger(L, 2);

    ESP_LOGI(TAG, "sent command %d to address: %d with data %d", command, address, motor_id);

    hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_MOTOR_DRVR, command, &motor_id, 1, NULL, 0);
}

static int32_t _get_counter(lua_State* L){
    LUA_PARAM_NR_CHECK(2);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t in_data[4];
    uint8_t motor_id = lua_tointeger(L, 2);
    if (!hugo_i2c_tiny(address, I2C_BLOCK_TYPE_ID_MOTOR_DRVR, I2C_MOTOR_GET_COUNTER, &motor_id, 1, in_data, 4)){
        ESP_LOGE(TAG, "no counter provided - for %d", address);
        return 0; //data are invalid it is better t to return a particular value than a random one
    }

    ESP_LOGI(TAG, "received_data %d %d %d %d", in_data[0], in_data[1], in_data[2], in_data[3]);

    int32_t counter = 0;
    for (uint8_t index = 0; index < 4; ++index){
        counter |= in_data[index] << index * 8;
    }

    return counter;
}

static int cl_motor_turn_clockwise(lua_State* L){
   _send_command(L, I2C_MOTOR_TURN_CLOCKWISE);
    return 0;
}

static int cl_motor_turn_anticlockwise(lua_State* L){
   _send_command(L, I2C_MOTOR_TURN_ANTICLOCKWISE);
    return 0;
}

static int cl_motor_stop(lua_State* L){
   _send_command(L, I2C_MOTOR_STOP);
    return 0;
}

static int cl_motor_power_off(lua_State* L){
   _send_command(L, I2C_MOTOR_POWER_OFF);
    return 0;
}

static int cl_motor_power_on(lua_State* L){
   _send_command(L, I2C_MOTOR_POWER_ON);
    return 0;
}

static int cl_motor_reset_counter(lua_State* L){
   _send_command(L, I2C_MOTOR_RESET_COUNTER);
    return 0;
}

static int cl_motor_get_counter(lua_State* L){
   lua_pushinteger(L, _get_counter(L));
   return 1;
}

static int cl_motor_speed_100(lua_State* L){
   _send_command(L, I2C_MOTOR_SET_SPEED_100);
    return 0;
}

static int cl_motor_speed_80(lua_State* L){
   _send_command(L, I2C_MOTOR_SET_SPEED_80);
    return 0;
}

static int cl_motor_speed_60(lua_State* L){
   _send_command(L, I2C_MOTOR_SET_SPEED_60);
    return 0;
}

static int cl_motor_speed_40(lua_State* L){
   _send_command(L, I2C_MOTOR_SET_SPEED_40);
    return 0;
}

static int cl_motor_speed_20(lua_State* L){
  _send_command(L, I2C_MOTOR_SET_SPEED_20);
    return 0;
}

static int cl_motor_speed_0(lua_State* L){
   _send_command(L, I2C_MOTOR_SET_SPEED_0);
    return 0;
}

bool tiny_motor_block_init(lua_State* L){
    REGISTER_LUA_FUNCTION(L, cl_motor_turn_clockwise);
    REGISTER_LUA_FUNCTION(L, cl_motor_turn_anticlockwise);
    REGISTER_LUA_FUNCTION(L, cl_motor_stop);
    REGISTER_LUA_FUNCTION(L, cl_motor_power_off);
    REGISTER_LUA_FUNCTION(L, cl_motor_power_on);
    REGISTER_LUA_FUNCTION(L, cl_motor_reset_counter);
    REGISTER_LUA_FUNCTION(L, cl_motor_get_counter);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_100);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_80);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_60);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_40);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_20);
    REGISTER_LUA_FUNCTION(L, cl_motor_speed_0);
    return true;
}