#pragma once
#include <fff.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <stdint.h>

void set_mock_ringbuffer_content(void *data, size_t entry_size, int count);
void *mock_xRingbufferReceive(RingbufHandle_t xRingbuffer, size_t *pxItemSize, TickType_t xTicksToWait);
