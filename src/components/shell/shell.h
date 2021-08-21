/*
 * Copyright (c) 2021 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

void hugo_shell_init();
bool hugo_shell_start_lua_main();

bool hugo_shell_process_command(
  uint8_t command_id, uint8_t const * in_data, size_t in_data_size, uint8_t ** out_data, size_t *out_data_size
);
bool hugo_shell_is_program_suspended();
