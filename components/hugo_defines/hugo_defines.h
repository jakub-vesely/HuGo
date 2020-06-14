#ifndef _HUGO_DEFINES_H_
#define _HUGO_DEFINES_H_

typedef int HUGO_RET_VAL;
#define HUGO_OK 0 /* success - no error */
#define HUGO_FAIL -1 /* generic error indication failure */

#define LUA_PARAM_NR_CHECK(expected) \
do { \
    int n = lua_gettop(L); \
    if(n != expected) \
    { \
        lua_pushstring(L, "unexpected number of arguments (expected "#expected")"); \
        lua_error(L); \
    } \
} while(0)

#define REGISTER_LUA_FUNCTUIN(L, name) lua_register(L, #name, name)

#endif //_HUGO_DEFINES_H_
