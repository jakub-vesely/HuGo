#include "event_loop.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <sys/lock.h>

#define TAG "EvnetLoop"

#define EVENT_LOOP_RING_BUFFER_SIZE 16
#define EVENT_ACTION_ARRAY_SIZE 32

typedef struct {
    event_prams_t event_ring_buffer[EVENT_LOOP_RING_BUFFER_SIZE];
    int ring_buffer_begin; // fist item pos
    int ring_buffer_end; // behind last item
    event_action_t event_action_array[EVENT_ACTION_ARRAY_SIZE];
    _lock_t lock;
    uint32_t event_id_counter; // if event loop will be used for timer and timer will be initialized every 100 ms it will take 13 years to reach max uint32
} loop_properties;


static loop_properties s_primary_loop_properties;

static loop_properties s_peripheral_loop_properties;

static loop_properties* _get_properties(int loop_type){
    return loop_type == EVENT_LOOP_TYPE_PRIMARY ? &s_primary_loop_properties : &s_peripheral_loop_properties;
}


void hugo_event_loop_init(int loop_type) {
    loop_properties *properties = _get_properties(loop_type);

    properties->ring_buffer_begin = 0;
    properties->ring_buffer_end = 0;
    properties->event_id_counter = 0;

    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i) {
        properties->event_action_array[i].used = false;
    }
}

int hugo_get_new_event_id(int loop_type) {
    loop_properties *properties = _get_properties(loop_type);

    _lock_acquire(&properties->lock);
    int id = -1;
    if (properties->event_id_counter == INT32_MAX) {
        ESP_LOGE(TAG, "Max event ID reached");
    }
    else {
        id = properties->event_id_counter;
        properties->event_id_counter++;
    }

    _lock_release(&properties->lock);
    return id;
}

bool hugo_add_event_action(int loop_type, int event_id, action_func_t action) {
    loop_properties *properties = _get_properties(loop_type);

    _lock_acquire(&properties->lock);
    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i) {
        event_action_t item = properties->event_action_array[i];
        if (!item.used && item.event_id == event_id && item.action == action) {
            ESP_LOGD(TAG, "Action for the event is already present");
            _lock_release(&properties->lock);
            return true;
        }
    }

    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i) {
        if (!properties->event_action_array[i].used) {
            properties->event_action_array[i].used = true;
            properties->event_action_array[i].event_id = event_id;
            properties->event_action_array[i].action = action;
            _lock_release(&properties->lock);
            return true;
        }
    }
    ESP_LOGE(TAG, "Event action array is full");
    _lock_release(&properties->lock);
    return false;
}


bool hugo_remove_event_action(int loop_type, int event_id, action_func_t action) {
    loop_properties *properties = _get_properties(loop_type);

    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i) {
        event_action_t item = properties->event_action_array[i];
        if (item.used && item.event_id == event_id && item.action == action) {
            properties->event_action_array[i].used = false;
            return true;
        }
    }
    ESP_LOGE(TAG, "Item has not been found in the event action array");
    return false;
}

static bool _ring_buffer_push(loop_properties *properties, uint8_t event_id, void* data, int data_size) {
    _lock_acquire(&properties->lock);
    int result = false;

    int index = properties->ring_buffer_end;
    int new_end = (properties->ring_buffer_end + 1) % EVENT_LOOP_RING_BUFFER_SIZE;
    if (new_end == properties->ring_buffer_begin) {
        ESP_LOGE(TAG, "Event loop's ring buffer is full");
    }
    else{
        if (data_size > EVENT_DATA_SIZE) {
            ESP_LOGE(TAG, "Ask to store too large event data %d > %d (event: %d)", data_size, EVENT_DATA_SIZE, event_id);
        }
        else{
            properties->event_ring_buffer[index].event_id = event_id;
            properties->event_ring_buffer[index].data_size = data_size;
            memcpy(properties->event_ring_buffer[index].data, data, data_size);
            properties->ring_buffer_end = new_end;
            result = true;
        }
    }
    _lock_release(&properties->lock);
    return result;
}

static bool _ring_buffer_pop(loop_properties *properties, uint32_t *event_id, void* data, uint8_t *data_size) {
    _lock_acquire(&properties->lock);

    bool result = false;
    if (properties->ring_buffer_begin == properties->ring_buffer_end) {
        ESP_LOGD(TAG, "Event loop's ring buffer is empty");
    }
    else{
        *event_id = properties->event_ring_buffer[properties->ring_buffer_begin].event_id;
        *data_size = properties->event_ring_buffer[properties->ring_buffer_begin].data_size;
        memcpy((uint8_t*)data, (uint8_t*)(properties->event_ring_buffer[properties->ring_buffer_begin].data), *data_size);
        properties->ring_buffer_begin = (properties->ring_buffer_begin + 1) % EVENT_LOOP_RING_BUFFER_SIZE;

        result = true;
    }

    _lock_release(&properties->lock);
    return result;
}

bool hugo_raise_event(int loop_type, uint32_t event_id, void* data, int data_size)
{
    loop_properties *properties = _get_properties(loop_type);
    bool ret_val = _ring_buffer_push(properties, event_id, data, data_size);
    return ret_val;
}

static void _process_buffer(int loop_type)
{
    loop_properties *properties = _get_properties(loop_type);

    uint32_t event_id = 0;
    event_data_t data;
    uint8_t data_size = 0;

    while (_ring_buffer_pop(properties, &event_id, data, &data_size)) {
        for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i) {
            event_action_t item = properties->event_action_array[i];
            if (item.used && item.event_id == event_id) {
                //ESP_LOGI(TAG, "raised event: %d", event_id);
                item.action(data, data_size);
            }
        }
    }

}

void hugo_process_events(int loop_type, bool exit_if_empty)
{
    while(true) {
        _process_buffer(loop_type);
        if (exit_if_empty) {
            return;
        }

        //FIXME: will be better vTaskSuspend istead?
        vTaskDelay(10); //tell to watchdog that the thread is still alive
    }
}
