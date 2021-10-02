#pragma once

#include <driver/rmt.h>
#include <esp_err.h>
#include <hal/rmt_types.h>

esp_err_t mock_rmt_get_ringbuf_handle(rmt_channel_t channel, RingbufHandle_t *buf_handle);
esp_err_t mock_rmt_rx_start(rmt_channel_t channel, bool rx_idx_rst);

