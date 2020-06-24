#pragma once

#include <esp_log.h>

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

#define REGISTER_LUA_FUNCTUIN(L, name) lua_register(L, #name, name)

#define CHECK_LUA_DOSTRING(L, string) do { \
    if (luaL_dostring(L, string)) { \
        ESP_LOGE(TAG, "luaL_dostring failed: %s\n", lua_tostring(L, -1)); \
        abort(); \
    } \
} while (0)

#define CHECK_LUA_GET_GLOBAL(L, name, n, r, f) do { \
    lua_getglobal(L, name); \
    if (lua_pcall(L, n, r, f)) { \
        ESP_LOGE(TAG, "lua_pcall failed: %s\n", lua_tostring(sL, -1)); \
        abort(); \
    } \
} while (0)

