/*
 * Copyright (c) 2020 jakub-vesely
 * This software is published under MIT license. Full text of the licence is available at https://opensource.org/licenses/MIT
 */

#ifndef _HUGO_EVENT_LOOP_H_
#define _HUGO_EVENT_LOOP_H_

#include <stdint.h>
#include <stdbool.h>

#define EVENT_DATA_SIZE 16 //to be possible to store two doubles

#define EVENT_LOOP_TYPE_PRIMARY 0 //loop for acttions performed main program
#define EVENT_LOOP_TYPE_PERIPHERAL 1 //loop for controling sensors and actors

typedef uint8_t event_data_t[EVENT_DATA_SIZE];
typedef struct
{
    uint32_t event_id;
    event_data_t data;
    uint8_t data_size; //to be safe
} event_prams_t;

typedef void (*action_func_t)(event_data_t data, int data_size);

typedef struct
{
    bool used;
    uint32_t event_id;
    action_func_t action;
} event_action_t;

void hugo_event_loop_init(int loop_type);
int hugo_get_new_event_id(int loop_type);
bool hugo_add_event_action(int loop_type, int32_t event_id, action_func_t action);
bool hugo_remove_event_action(int loop_type, int32_t event_id, action_func_t action);
bool hugo_raise_event(int loop_type, uint32_t event_id, void* data, int data_size);
void hugo_process_events(int loop_type, bool exit_if_empty);
#endif //HUGO_EVENT_LOOP_H_
