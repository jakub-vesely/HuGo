idf_component_register(
    SRCS  ./MPU9250_fork/src/MPU9250.cpp ./mpu9250_wrapper.cpp
    INCLUDE_DIRS .
    REQUIRES arduino-esp32
)

