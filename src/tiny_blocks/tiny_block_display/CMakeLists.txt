idf_component_register(
    SRCS tiny_block_display.c graphics.c ssd1306.c
    INCLUDE_DIRS . REQUIRES i2c lua hugo_defines event_loop
)
