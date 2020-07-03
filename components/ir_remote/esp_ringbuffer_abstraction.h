#ifdef UNIT_TEST
#   include <esp_ringbuffer_mocks.h>
#   define X_RINGBUFFER_RECEIVE(xRingbuffer, pxItemSize, xTicksToWait) mock_xRingbufferReceive(xRingbuffer, pxItemSize, xTicksToWait)
#   define V_RINGBUFFER_RETURN_ITEM(xRingbuffer, pvItem) ;
#else
#   include <freertos/FreeRTOS.h>
#   include <freertos/ringbuf.h>
#   define X_RINGBUFFER_RECEIVE(xRingbuffer, pxItemSize, xTicksToWait) xRingbufferReceive(xRingbuffer, pxItemSize, xTicksToWait)
#   define V_RINGBUFFER_RETURN_ITEM(xRingbuffer, pvItem) vRingbufferReturnItem(xRingbuffer, pvItem);
#endif
