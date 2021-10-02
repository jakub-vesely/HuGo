#ifdef UNIT_TEST
#   include <driver_rtm_mocks.h>
#   define RMT_GET_RINGBUF_HANDLE(channel, buf_handle) mock_rmt_get_ringbuf_handle(channel, buf_handle)
#   define RMT_RX_START(channel, rx_idx_rst) mock_rmt_rx_start(channel, rx_idx_rst)
#else
#   include <driver/rmt.h>
#   define RMT_GET_RINGBUF_HANDLE(channel, buf_handle) rmt_get_ringbuf_handle(channel, buf_handle)
#   define RMT_RX_START(channel, rx_idx_rst) rmt_rx_start(channel, rx_idx_rst)
#endif
