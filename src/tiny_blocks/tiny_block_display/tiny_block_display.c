#include "tiny_block_display.h"
#include "display_adapter.h"
#include "graphics.h"
#include "ssd1306.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <hugo_defines.h>
#include <i2c.h>
#include <hugo_tiny_block_defines.h>
#include "tiny_block_display_arduino/tiny_block_display_arduino.h"

typedef struct block_properties_t{
    uint8_t block_address;
    display_adapter_t* adapter;
} block_properties_t;

//ony 2 ssd1306 can be placed on one i2c (one addres pin)
static block_properties_t s_blocks[2] = {{I2C_ADDRESS_NONE, NULL}, {I2C_ADDRESS_NONE, NULL}};

static const char* TAG = "display";

static bool _initialize(uint8_t block_address){
    if (s_blocks[1].block_address != I2C_ADDRESS_NONE){
        ESP_LOGE(TAG, "try to initialize third display (only 2 are supported)");
        return false;
    }

    if (s_blocks[0].block_address == block_address || s_blocks[1].block_address == block_address){
        ESP_LOGE(TAG, "block with address %d has already been initialized", block_address);
        return false;
    }

    int8_t index = 0;
    if (s_blocks[0].block_address != I2C_ADDRESS_NONE){
        index = 1;
    }

    ESP_LOGI(TAG, "address %d asigned to diplay block with index %d", block_address, index);

    uint8_t data[3];
    if (!hugo_i2c_tiny(block_address, I2C_BLOCK_TYPE_ID_DISPLAY, I2C_COMMAND_DISP_GET_PARAMS, NULL, 0, data, 3)){
        ESP_LOGE(TAG, "display params was not provided- for block %d", block_address);
    }
    uint8_t display_address = data[0];
    uint8_t width = data[1];
    uint8_t height = data[2];
    ESP_LOGI(TAG, "received dis_address: %d, width: %d, heigh: %d", display_address, width, height);

    if (index == 1 && s_blocks[0].adapter->address == display_address){
        ESP_LOGE(TAG, "display with addres %d is already initialized", display_address);
        return false;
    }

    display_adapter_t *adapter = hugo_ssd1306_init(display_address, width, height, false);
    if (adapter == NULL){
        ESP_LOGE(TAG, "display with addres %d has neo been initialized", display_address);
        return false;
    }

    //display actions are performed in the peperipherial contxt
    while (!adapter->initialized){
        vTaskDelay(10); //tell to watchdog that the thread is still alive
    }

    s_blocks[index].block_address = block_address;
    s_blocks[index].adapter = adapter;
    ESP_LOGI(TAG, "initialized display_block with addreses %d, %d", block_address, display_address);
    return true;
}

static int cl_display_initialize(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);
    uint8_t block_address = lua_tointeger(L, 1);
    lua_pushinteger(L, _initialize(block_address));
    return 1;
}

static int8_t _get_index(uint8_t block_address){
    if (s_blocks[0].block_address == block_address){
        return 0;
    }
    if (s_blocks[1].block_address == block_address){
        return 1;
    }
    return -1;
}

static int cl_display_get_ssd_i2c_address(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for get_ssd_i2c_address", block_address);
        lua_pushinteger(L, I2C_ADDRESS_NONE);
    }
    else{
        lua_pushinteger(L, s_blocks[index].adapter->address);
    }
    return 1;
}

static int cl_display_get_ssd_dimensions(lua_State* L) {
    LUA_PARAM_NR_CHECK(1);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for get_ssd_dimensions", block_address);
        lua_pushinteger(L, -1);
        lua_pushinteger(L, -1);
    }
    lua_pushinteger(L, s_blocks[index].adapter->width);
    lua_pushinteger(L, s_blocks[index].adapter->height);

    return 2;
}

static bool set_ssd_a0(uint8_t block_address, uint8_t a0){
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for set_ssd_a0", block_address);
        return false;
    }

    uint8_t display_address = s_blocks[index].adapter->address;
    if (!hugo_i2c_tiny(display_address, I2C_BLOCK_TYPE_ID_POWER, I2C_COMMAND_DISP_SET_SSD_A0, &a0, 1, NULL, 0)){
        ESP_LOGE(TAG, "SSD a0 was not written - for %d", block_address);
        return false;
    }
    s_blocks[index].adapter->address = a0 ? INA_I2C_ADDRESS_A0 : INA_I2C_ADDRESS_NA0;
    return true;
}

static int cl_display_set_ssd_a0(lua_State* L) {
    LUA_PARAM_NR_CHECK(2);
    uint8_t block_address = lua_tointeger(L, 1);
    uint8_t a0 = lua_tointeger(L, 2);
    lua_pushinteger(L, set_ssd_a0(block_address, a0));
    return 1;
}

static int cl_display_showtime(lua_State *L)
{
    LUA_PARAM_NR_CHECK(1);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for showtime", block_address);
    }
    else{
        s_blocks[index].adapter->showtime(s_blocks[index].adapter);
    }

    lua_pushinteger(L, index != -1);
    return 1;
}

static int cl_display_clean(lua_State *L)
{
    LUA_PARAM_NR_CHECK(1);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for clean", block_address);
    }
    else{
        s_blocks[index].adapter->clean(s_blocks[index].adapter);
    }

    lua_pushinteger(L, index != -1);
    return 1;
}

static int cl_display_draw_point(lua_State *L)
{
    LUA_PARAM_NR_CHECK(4);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for draw point", block_address);
    }
    else {
        hugo_graphics_set_point(s_blocks[index].adapter, lua_tointeger(L, 2), lua_tointeger(L, 3), lua_toboolean(L, 4));
    }

    lua_pushinteger(L, index != -1);
    return 1;
}

static int cl_display_draw_elipse(lua_State *L)
{
    LUA_PARAM_NR_CHECK(6);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for draw elipse", block_address);
    }
    else {
        hugo_graphics_draw_elipse(
            s_blocks[index].adapter,
            lua_tointeger(L, 2),
            lua_tointeger(L, 3),
            lua_tointeger(L, 4),
            lua_tointeger(L, 5),
            lua_toboolean(L, 6)
        );
    }
    lua_pushinteger(L, index != -1);
    return 1;
}

static int cl_display_print_text_8x8(lua_State *L)
{
    LUA_PARAM_NR_CHECK(7);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for print text8x8", block_address);
    }
    else {
        hugo_graphics_print_text_8x8(
            s_blocks[index].adapter,
            lua_tointeger(L, 2),
            lua_tointeger(L, 3),
            lua_tointeger(L, 4),
            lua_tointeger(L, 5),
            (char*)lua_tostring(L, 6),
            lua_toboolean(L, 7)
        );
    }
    lua_pushinteger(L, index != -1);
    return 1;
}

static int cl_display_print_text_8x16(lua_State *L)
{
    LUA_PARAM_NR_CHECK(7);
    uint8_t block_address = lua_tointeger(L, 1);
    int8_t index = _get_index(block_address);
    if (index == -1){
        ESP_LOGE(TAG, "unknown address %d for print text8x16", block_address);
    }
    else {
        hugo_graphics_print_text_8x16(
            s_blocks[index].adapter,
            lua_tointeger(L, 1),
            lua_tointeger(L, 2),
            lua_tointeger(L, 3),
            lua_tointeger(L, 4),
            (char*)lua_tostring(L, 5),
            lua_toboolean(L, 6)
        );
    }
    lua_pushinteger(L, index != -1);
    return 1;
}

void tiny_display_init(lua_State* L, bool invert)
{
    REGISTER_LUA_FUNCTION(L, cl_display_initialize);
    REGISTER_LUA_FUNCTION(L, cl_display_get_ssd_i2c_address);
    REGISTER_LUA_FUNCTION(L, cl_display_get_ssd_dimensions);
    REGISTER_LUA_FUNCTION(L, cl_display_set_ssd_a0);
    REGISTER_LUA_FUNCTION(L, cl_display_showtime);
    REGISTER_LUA_FUNCTION(L, cl_display_clean);
    REGISTER_LUA_FUNCTION(L, cl_display_draw_point);
    REGISTER_LUA_FUNCTION(L, cl_display_draw_elipse);
    REGISTER_LUA_FUNCTION(L, cl_display_print_text_8x8);
    REGISTER_LUA_FUNCTION(L, cl_display_print_text_8x16);

    /*hugo_graphics_print_text_8x8(
        s_blocks[0].adapter,
        1, 0, 64, 48,
        //"Hello",
        "A1A2A3a4\nB1B2B3b\nC1C2C3c\nD1D2D3d\nE1E2E3e\nF1F2F3f4",
        true
    );*/
    /*hugo_graphics_print_text_8x8(
        1, 10, 64, 48,
        //"Hello",
        "B1B2B3b\nC1C2C3c\nD1D2D3d\nE1E2E3e",
        true
    );

    hugo_graphics_print_text_8x8(
        1, 10, 64, 48,
        //"Hello",
        "B1B2B3b\nC1C2C3c\nD1D2D3d\nE1E2E3e",
        true
    );*/


    /*hugo_graphics_print_text8x16(
        0, 0, 128, 64,
        "H",
        true
    );
*/
    /*hugo_graphics_print_text8x8(
         0, 0, 128, 64,
         "  Hello World!\nSecond line.\nloooooooooong line\n\n\n\nLast line\n out of range",
         true);
*/
    //hugo_graphics_draw_elipse(64,32, 42, 22, true);

    //_draw_elipse(34, 32, 28, 18);
    //_draw_elipse(94, 32, 28, 18);

    //_draw_elipse(34, 32, 8, 8);
    //_draw_elipse(94, 32, 8, 8);

    /*_draw_point(0, 0, false);
    _draw_point(127, 0, false);
    _draw_point(0, 63, false);
    _draw_point(127, 63, false);
    _draw_point(63, 31, false);
    */
    //_adapter->showtime();
}
