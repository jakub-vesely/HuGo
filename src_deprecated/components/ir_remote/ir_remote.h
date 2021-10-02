/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#ifndef _HUGO_IR_REMOTE_H_
#define _HUGO_IR_REMOTE_H_

#include "lua.h"

void hugo_ir_remote_init_module(lua_State* L, int ir_remote_pin);
void hugo_ir_remote_start_listening();
void hugo_ir_remote_stop_listening();

#endif //_HUGO_IR_REMOTE_H_
