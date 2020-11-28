#pragma once

#include <esp_log.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

typedef int HUGO_RET_VAL;
#define HUGO_OK 0 /* success - no error */
#define HUGO_FAIL -1 /* generic error indication failure */

#define LUA_PARAM_NR_CHECK(expected) do { \
    int n = lua_gettop(L); \
    if(n != expected) \
    { \
        lua_pushstring(L, "unexpected number of arguments (expected "#expected")"); \
        lua_error(L); \
    } \
} while(0)

#define REGISTER_LUA_FUNCTION(L, name) lua_register(L, #name, name)

#define CHECK_LUA_DOSTRING(L, string) do { \
    if (luaL_dostring(L, string)) { \
        ESP_LOGE(TAG, "luaL_dostring failed: %s\n", lua_tostring(L, -1)); \
        abort(); \
    } \
} while (0)

#define CHECK_LUA_GET_GLOBAL(L, name, n, r, f) do { \
    lua_getglobal(L, name); \
    if (lua_pcall(L, n, r, f)) { \
        ESP_LOGE(TAG, "lua_pcall failed: %s\n", lua_tostring(L, -1)); \
        abort(); \
    } \
} while (0)


/* used mainly in tests. For the case that previous test did not pass and
 * after_each was not called is present tested whether lua is already defined.
 * If so this instance closed first to be tests independent.
*/
#define LUA_REOPEN(L) do { \
    if (L != NULL) lua_close(L); \
    L = (lua_State*)luaL_newstate(); \
    luaL_openlibs(L); \
} while (0)

#define LUA_CLOSE(L) do { \
    lua_close(L); \
    L = NULL; \
} while(0)
