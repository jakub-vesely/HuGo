idf_component_register(
    SRCS  ./mpu9250_lua_interface.c
    INCLUDE_DIRS .
    REQUIRES  hugo_defines lua Bolder_Flight_Systems_MPU9250 timer event_loop
)

