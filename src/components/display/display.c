#include "display.h"
#include "display_adapter.h"
#include "graphics.h"
#include "ssd1306.h"

#include <hugo_defines.h>

static display_adapter_t *_adapter = NULL;

static int cl_display_showtime(lua_State *L)
{
    LUA_PARAM_NR_CHECK(0);
    _adapter->showtime();
    return 0;
}

static int cl_display_clean(lua_State *L)
{
    LUA_PARAM_NR_CHECK(0);
    _adapter->clean();
    return 0;
}

static int cl_display_draw_point(lua_State *L)
{
    LUA_PARAM_NR_CHECK(3);
    hugo_graphics_set_point(lua_tointeger(L, 1), lua_tointeger(L, 2), lua_toboolean(L, 3));
    return 0;
}

static int cl_display_draw_elipse(lua_State *L)
{
    LUA_PARAM_NR_CHECK(5);
    hugo_graphics_draw_elipse(
        lua_tointeger(L, 1),
        lua_tointeger(L, 2),
        lua_tointeger(L, 3),
        lua_tointeger(L, 4),
        lua_toboolean(L, 5)
    );
    return 0;
}

static int cl_display_print_text_8x8(lua_State *L)
{
    LUA_PARAM_NR_CHECK(6);
    hugo_graphics_print_text_8x8(
        lua_tointeger(L, 1),
        lua_tointeger(L, 2),
        lua_tointeger(L, 3),
        lua_tointeger(L, 4),
        (char*)lua_tostring(L, 5),
        lua_toboolean(L, 6)
    );
    return 0;
}

static int cl_display_print_text_8x16(lua_State *L)
{
    LUA_PARAM_NR_CHECK(6);
    hugo_graphics_print_text_8x16(
        lua_tointeger(L, 1),
        lua_tointeger(L, 2),
        lua_tointeger(L, 3),
        lua_tointeger(L, 4),
        (char*)lua_tostring(L, 5),
        lua_toboolean(L, 6)
    );
    return 0;
}

void hugo_display_init(lua_State* L, bool invert)
{
    _adapter = hugo_ssd1306_init(invert);
    hugo_graphics_init(_adapter);

    REGISTER_LUA_FUNCTION(L, cl_display_showtime);
    REGISTER_LUA_FUNCTION(L, cl_display_clean);
    REGISTER_LUA_FUNCTION(L, cl_display_draw_point);

    REGISTER_LUA_FUNCTION(L, cl_display_draw_elipse);
    REGISTER_LUA_FUNCTION(L, cl_display_print_text_8x8);
    REGISTER_LUA_FUNCTION(L, cl_display_print_text_8x16);

    /*hugo_graphics_print_text8x16(
        0, 0, 128, 64,
        " Hello World!\nSecond loooooooooong line\nLast line\n out of range",
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
    //adapter->showtime();
}
