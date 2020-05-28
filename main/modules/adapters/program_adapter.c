#include "program_adapter.h"

HUGO_RET_VAL connect_to_started(lua_State* L)
{
    return HUGO_OK;
}

HUGO_RET_VAL create_program_adapter(lua_State* L)
{
    lua_register(L, "connect_to_started", connect_to_started_action);
    return HUGO_OK;
}
