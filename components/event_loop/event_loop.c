#include "event_loop.h"
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>
#include <sys/lock.h>

#define EVENT_LOOP_RING_BUFFER_SIZE 32
#define EVENT_ACTION_ARRAY_SIZE 32
#define TAG "EvnetLoop"

static event_prams_t s_event_ring_buffer[EVENT_LOOP_RING_BUFFER_SIZE];
static int s_ring_buffer_begin = 0; // fist item pos
static int s_ring_buffer_end = 0; // behind last item
static event_action_t s_event_action_array[EVENT_ACTION_ARRAY_SIZE];
static _lock_t s_lock;
static uint32_t s_event_id_counter = 0; // if event loop will be used for timer and timer will be initialized every 100 ms it will take 13 years to reach max uint

void init()
{
    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i)
    {
        s_event_action_array[i].used = false;
    }
}

int get_new_event_id()
{
    _lock_acquire(&s_lock);
    int id = -1;
    if (s_event_id_counter == INT32_MAX)
    {
        ESP_LOGE(TAG, "Max evnet ID reached");
    }
    else
    {
        id = s_event_id_counter;
        s_event_id_counter++;
    }

    _lock_release(&s_lock);
    return id;
}

bool add_event_action(int event_id, action_func_t action)
{
    _lock_acquire(&s_lock);
    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i)
    {
        event_action_t item = s_event_action_array[i];
        if (!item.used && item.event_id == event_id && item.action == action)
        {
            ESP_LOGD(TAG, "Action for the event is already present");
            _lock_release(&s_lock);
            return true;
        }
    }

    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i)
    {
        if (!s_event_action_array[i].used)
        {
            s_event_action_array[i].used = true;
            s_event_action_array[i].event_id = event_id;
            s_event_action_array[i].action = action;
            _lock_release(&s_lock);
            return true;
        }
    }
    ESP_LOGE(TAG, "Event action array is full");
    _lock_release(&s_lock);
    return false;
}

bool remove_event_action(int event_id, action_func_t action)
{
    for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i)
    {
        event_action_t item = s_event_action_array[i];
        if (item.used && item.event_id == event_id && item.action == action)
        {
            s_event_action_array[i].used = false;
            return true;
        }
    }
    ESP_LOGE(TAG, "Item has not been found in the event action array");
    return false;
}

static bool _ring_buffer_push(uint8_t event_id, void* data, int data_size)
{
    int new_end = (s_ring_buffer_end + 1) % EVENT_LOOP_RING_BUFFER_SIZE;
    if (new_end == s_ring_buffer_begin)
    {
        ESP_LOGE(TAG, "Evnet loop's ring buffer is full");
        return false;
    }
    if (data_size > EVENT_DATA_SIZE)
    {
        ESP_LOGE(TAG, "Ask to store too large event data");
        return false;
    }

    int index = new_end - 1;
    s_event_ring_buffer[index].event_id = event_id;
    s_event_ring_buffer[index].data_size = data_size;
    memcpy(s_event_ring_buffer[index].data, data, data_size);
    s_ring_buffer_end = new_end;
    return true;
}

static bool _ring_buffer_pop(uint32_t *event_id, void* data, uint8_t *data_size)
{
    _lock_acquire(&s_lock);

    if (s_ring_buffer_begin == s_ring_buffer_end)
    {
        ESP_LOGD(TAG, "Evnet loop's ring buffer is empty");

        _lock_release(&s_lock);
        return false;
    }
    *event_id = s_event_ring_buffer[s_ring_buffer_begin].event_id;
    *data_size = s_event_ring_buffer[s_ring_buffer_begin].data_size;
    memcpy((uint8_t*)data, (uint8_t*)(s_event_ring_buffer[s_ring_buffer_begin].data), *data_size);
    s_ring_buffer_begin++;

    _lock_release(&s_lock);
    return true;
}

bool raise_event(uint32_t event_id, void* data, int data_size)
{
    _lock_acquire(&s_lock);
    bool ret_val = _ring_buffer_push(event_id, data, data_size);
    _lock_release(&s_lock);
    return ret_val;
}
static void _process_buffer()
{
    uint32_t event_id = 0;
    event_data_t data;
    uint8_t data_size = 0;

    while (_ring_buffer_pop(&event_id, data, &data_size))
    {
        for (int i = 0; i < EVENT_ACTION_ARRAY_SIZE; ++i)
        {
            event_action_t item = s_event_action_array[i];
            if (item.used && item.event_id == event_id)
            {
                item.action(data, data_size);
            }
        }
    }
}

bool process_events()
{
    while(true)
    {
        _process_buffer();
        //FIXME: will be better vTaskSuspend istead?
        vTaskDelay(10); //tell to watchdog that the thread is still alive
    }
}
