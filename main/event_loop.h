#ifndef _EVENT_LOOP_H
#define _EVENT_LOOP_H

#include "esp_event.h"

ESP_EVENT_DECLARE_BASE(TASK_EVENTS);

void start_event_loop();
void add_reaction_to_action(void (*action_method)(), esp_event_handler_t reaction_method);
void perform_action(void (*action_method)() , void* data, int data_size);

#endif //_EVENT_LOOP_H
