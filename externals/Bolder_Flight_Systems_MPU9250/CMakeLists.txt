idf_component_register(
    SRCS  ./Bolder_Flight_Systems_MPU9250/src/MPU9250.cpp ./mpu9250_wrapper.cpp
    INCLUDE_DIRS .
    REQUIRES arduino-esp32
)

