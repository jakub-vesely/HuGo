#include "esp_ringbuffer_mocks.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static void* _data = NULL;
static size_t _entry_size = 0;
static int _count = 0;
static int _position = 0;

void set_mock_ringbuffer_content(void *data, size_t entry_size, int count)
{
    _data = data;
    _entry_size = entry_size;
    _count = count;
}

void *mock_xRingbufferReceive(RingbufHandle_t xRingbuffer, size_t *pxItemSize, TickType_t xTicksToWait)
{

    if (_position == _count)
    {
        vTaskDelay(1);
        return NULL;
    }

    *pxItemSize = _entry_size;
    return _data + _position++ * _entry_size;
}
