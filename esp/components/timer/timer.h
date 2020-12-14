/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#pragma once

#include <lua.h>
#include <stdbool.h>
#include <esp_timer.h>

int hugo_create_new_timer(esp_timer_cb_t callback_function);
bool hugo_start_timer_once(int timer_id, int time_us);
void hugo_timer_init_module(lua_State* L);

