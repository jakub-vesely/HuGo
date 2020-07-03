/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#ifndef _HUGO_CHASSIS_H_
#define _HUGO_CHASSIS_H_

#include <lua.h>

void hugo_chassis_init_module(lua_State* L, int left_front_pin, int left_back_pin, int right_front_pin, int right_back_pin);

#endif //_HUGO_CHASSIS_H_
