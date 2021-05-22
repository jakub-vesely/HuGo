
#include "tiny_block_power.h"

#include <lua.h>
#include <lualib.h>
#include <hugo_defines.h>
#include <i2c.h>
#include <string.h>
#include "tiny_block_power_arduino/tiny_block_power_arduino.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static uint8_t s_data[2];
static const char *TAG = "power";

#define SHUNT_R 0.1

static bool _get_value(uint8_t address, uint8_t command, uint8_t* out_data, uint8_t out_data_size){

    hugo_i2c_write_command(address, command);
    //vTaskDelay(100); //tell to watchdog that the thread is still alive
    return hugo_i2c_read_data(address, out_data, out_data_size);
}

static int cl_power_is_charging(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t address = lua_tointeger(L, 1);
    if (!_get_value(address, I2C_COMMAND_GET_CHARGING_STATE,  s_data, 1)){
        ESP_LOGE(TAG, "charging state was not provided - for %d", address);
        s_data[0] = 0; //data are invalid it is better to return a particular value than a random one
    }
    lua_pushboolean(L, (bool)s_data[0]);

    return 1;
}

static int cl_power_get_ina_i2c_address(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t address = lua_tointeger(L, 1);
    if (!_get_value(address, I2C_COMMAND_GET_INA_I2C_ADDRESS,  s_data, 1)){
        ESP_LOGE(TAG, "INA I2C address was not provided- for %d", address);
        s_data[0] = 0; //data are invalid it is better to return a particular value than a random one
    }
    lua_pushinteger(L, s_data[0]);

    return 1;
}

static int cl_power_set_ina_a0a1(lua_State* L) {
    LUA_PARAM_NR_CHECK(2);

    uint8_t address = lua_tointeger(L, 1);
    uint8_t a0a1 = lua_tointeger(L, 2);
    if (!hugo_i2c_write_command_with_data(address, I2C_COMMAND_SET_INA_A0A1,  &a0a1, 1)){
        ESP_LOGE(TAG, "INA a0a1 was not written - for %d", address);
    }

    return 0;
}

static int cl_power_get_voltage(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t ina_address = lua_tointeger(L, 1);
    hugo_i2c_write_command(ina_address, I2C_COMMAND_INA219_BUSVOLTAG);

    if (!hugo_i2c_read_data(ina_address, s_data, 2)){
        memset(s_data, 0, 2);
        ESP_LOGE(TAG, "no voltage provided - for %d", ina_address);
    }

    uint16_t raw_voltage = ((s_data[0] << 8) | s_data[1]) >> 3;
    double voltage = (double)raw_voltage * 0.004; //LSB = 4 mV

    lua_pushnumber(L, voltage);
    return 1;
}

static int cl_power_get_current_ma(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);

    uint8_t ina_address = lua_tointeger(L, 1);
    hugo_i2c_write_command(ina_address, I2C_COMMAND_INA219_SHUNT_VOLTAGE);

    if (!hugo_i2c_read_data(ina_address, s_data, 2)){
        memset(s_data, 0, 2);
        ESP_LOGE(TAG, "no shunt voltage provided - for %d", ina_address);
    }

    int16_t raw_voltage = (s_data[0] << 8) | s_data[1];
    double current = (double)raw_voltage * 0.01 / SHUNT_R; //LSB = 10 uV

    lua_pushnumber(L, current);
    return 1;
}

static int cl_power_initialize_ina(lua_State* L){

    //float max_expected_current = 2;
    uint8_t ina_address = lua_tointeger(L, 1);
    config_reg_t config = {
        .mode = INA219_MODE_SHUNT_BUS_CONTINUOUS,
        .sadc = INA219_SADC_8SAM_4260,
        .badc = INA219_BADC_RES_12BIT,
        .pg = INA219_PG_GD4_160MV,
        .brng = INA219_BRNG_16V,
        .none = 0,
        .rst = 0,
    };

    hugo_i2c_write_command_with_data(ina_address, I2C_COMMAND_INA219_COFIGURATION, (uint8_t*)&config, 2);

    return 0;
}

bool tiny_power_init(lua_State* L) {
    REGISTER_LUA_FUNCTION(L, cl_power_is_charging);
    REGISTER_LUA_FUNCTION(L, cl_power_get_ina_i2c_address);
    REGISTER_LUA_FUNCTION(L, cl_power_set_ina_a0a1);
    REGISTER_LUA_FUNCTION(L, cl_power_initialize_ina);
    REGISTER_LUA_FUNCTION(L, cl_power_get_voltage);
    REGISTER_LUA_FUNCTION(L, cl_power_get_current_ma);
    return true;
}
