#include "driver_rtm_mocks.h"
#include <freertos/ringbuf.h>

static RingbufHandle_t *s_buf_handle = NULL;

esp_err_t mock_rmt_get_ringbuf_handle(rmt_channel_t channel, RingbufHandle_t *buf_handle)
{
    esp_err_t ret_val = rmt_get_ringbuf_handle(channel,  buf_handle);
    s_buf_handle = buf_handle;
    return ret_val;
}

esp_err_t mock_rmt_rx_start(rmt_channel_t channel, bool rx_idx_rst)
{
    return ESP_OK;
}
