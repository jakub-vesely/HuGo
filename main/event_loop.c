#include "event_loop.h"
#include "esp_event_base.h"

esp_event_loop_handle_t event_loop;

ESP_EVENT_DEFINE_BASE(TASK_EVENTS);

void start_event_loop()
{
    esp_event_loop_args_t event_loop_args = {
        .queue_size = 10,
        .task_name = "event_loop_task", // task will be created
        .task_priority = uxTaskPriorityGet(NULL),
        .task_stack_size = 2048,
        .task_core_id = tskNO_AFFINITY
    };

     // Create the event loop
    ESP_ERROR_CHECK(esp_event_loop_create(&event_loop_args, &event_loop));

}

void add_reaction_to_action(void (*action_method)(), esp_event_handler_t reaction_method)
{
    //there is expected that pointer size is 32bit in esp-idf
    ESP_ERROR_CHECK(esp_event_handler_instance_register_with(
        event_loop, TASK_EVENTS, (int32_t)action_method, reaction_method, event_loop, NULL
    ));
}

void perform_action(void (*action_method)(), void* data, int data_size)
{
    ESP_ERROR_CHECK(esp_event_post_to(event_loop, TASK_EVENTS, (int32_t)action_method, data, data_size, portMAX_DELAY));
}
