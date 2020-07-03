
idf_component_register(
    SRCS driver_gpio_mocks.c driver_rtm_mocks.c gpio_mocks.c esp_ringbuffer_mocks.c
    INCLUDE_DIRS .
    REQUIRES fff gpio infrared_tools)
